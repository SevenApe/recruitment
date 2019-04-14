/*
   Copyright (C) 2010 Marco Ballesio <gibrovacco@gmail.com>
   Copyright (C) 2011-2012 Collabora Ltd.
     @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "player.h"
#include <QUrl>
#include <QFileDialog>
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/ElementFactory>
#include <QGst/Bus>
#include <QGst/query.h>
#include <QGst/pipeline.h>
#include <QGst/Event>


Player::Player(QObject *parent)
    : QObject(parent)
{
    duration = "Duration: 00:00:00.000";
    position = "Position: 00:00:00.000";
    slider_position = 0;

    // When this timer triggers it will update the position text
    connect(&position_timer, SIGNAL(timeout()), this, SLOT(updatePosition()));
}

void Player::setVideoSink(const QGst::ElementPtr & sink)
{
    m_videoSink = sink;
}

void Player::play()
{
    if (m_pipeline) {
        m_pipeline->setState(QGst::StatePlaying);
    }
}

void Player::stop()
{
    if (m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
        position_timer.stop();

        // Reset the position clock
        setPosition(QTime(0,0));
    }
}

void Player::open()
{
    // parent() is the QDeclarativeView here
    QString fileName = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(parent()),
                                                    tr("Open a Movie"), m_baseDir);

    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void Player::openFile(const QString & fileName)
{
    m_baseDir = QFileInfo(fileName).path();

    stop();
    setUri(QUrl::fromLocalFile(fileName).toEncoded());
    play();
}

void Player::setUri(const QString & uri)
{
    if (!m_pipeline) {
        m_pipeline = QGst::ElementFactory::make("playbin").dynamicCast<QGst::Pipeline>();
        if (m_pipeline) {
            m_pipeline->setProperty("video-sink", m_videoSink);

            //watch the bus for messages
            QGst::BusPtr bus = m_pipeline->bus();
            bus->addSignalWatch();
            QGlib::connect(bus, "message", this, &Player::onBusMessage);
        } else {
            qCritical() << "Failed to create the pipeline";
        }
    }

    if (m_pipeline) {
        m_pipeline->setProperty("uri", uri);
    }

}

void Player::sliderPositionChanged(const double &position)
{
    QTime *duration = media_info_gatherer.getDuration();

    // Get the total number of seconds in the media
    double duration_seconds = QTime(0, 0, 0).secsTo(*duration);

    // Calculate how many seconds in the position of the slider is
    double position_seconds = duration_seconds * position;

    // Convert seconds back to QTime
    QTime new_position = QTime(0,0,0);
    new_position = new_position.addSecs(position_seconds);

    // Seek to the new position
    seekToPosition(new_position);
}


QString Player::getDuration()
{
    return duration;
}

void Player::setDuration(QTime *duration_qtime)
{
    duration = "Duration: " + duration_qtime->toString("HH:mm:ss.zzz");
    Q_EMIT durationUpdated();
}

QString Player::getPosition()
{
    return position;
}

double Player::getSliderPosition()
{
    return slider_position;
}

void Player::setPosition(QTime position_qtime)
{
    position = "Position: " + position_qtime.toString("HH:mm:ss.zzz");
    QTime *duration = media_info_gatherer.getDuration();

    double duration_seconds = QTime(0, 0, 0).secsTo(*duration);
    double position_seconds = QTime(0, 0, 0).secsTo(position_qtime);

    slider_position = position_seconds / duration_seconds;

    Q_EMIT positionUpdated();
}

void Player::updatePosition()
{
    setPosition(media_info_gatherer.getPosition(m_pipeline));
}

void Player::seekToPosition(QTime seek_to)
{
    QGst::SeekEventPtr evt = QGst::SeekEvent::create(
        1.0, QGst::FormatTime, QGst::SeekFlagFlush,
        QGst::SeekTypeSet, QGst::ClockTime::fromTime(seek_to),
        QGst::SeekTypeNone, QGst::ClockTime::None
    );
    m_pipeline->sendEvent(evt);
}


void Player::onBusMessage(const QGst::MessagePtr & message)
{
    switch (message->type()) {
    case QGst::MessageStreamStart:
        // Get information about the stream
        media_info_gatherer.getInfo(m_pipeline);
        // Update the UI elements with stream info
        setDuration(media_info_gatherer.getDuration());
        //Check the position of the stream every 20 ms
        position_timer.start(20);
        break;
    case QGst::MessageEos: //End of stream. We reached the end of the file.
        stop();
        break;
    case QGst::MessageError: //Some error occurred.
        qCritical() << message.staticCast<QGst::ErrorMessage>()->error();
        stop();
        break;
    default:
        break;
    }
}

#include "moc_player.cpp"
