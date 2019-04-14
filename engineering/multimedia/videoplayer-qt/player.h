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
#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTimer>

#include <QGst/Pipeline>
#include <QGst/Message>

#include "mediainfogatherer.h"

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString duration READ getDuration NOTIFY durationUpdated)
    Q_PROPERTY(QString position READ getPosition NOTIFY positionUpdated)


public:
    explicit Player(QObject *parent = 0);
    void setVideoSink(const QGst::ElementPtr & sink);

    /**
     * @brief getDuration - Called from the qml to get the
     * duration; triggered by the durationUpdated signal.
     * @return
     */
    QString getDuration();

    /**
     * @brief getPosition - Called from qml to get the position
     * triggered by the positionUpdated signal.
     * @return
     */
    QString getPosition();

    void seekToPosition(QTime);

public Q_SLOTS:
    void play();
    void stop();
    void open();

    void sliderPositionChanged(const double &position);

private Q_SLOTS:
    /**
     * @brief updatePosition - Called every 20ms
     * by position_timer when the stream is playing.
     */
    void updatePosition();

Q_SIGNALS:
    void durationUpdated();
    void positionUpdated();

private:
    void openFile(const QString & fileName);
    void setUri(const QString & uri);
    void onBusMessage(const QGst::MessagePtr & message);

    /**
     * @brief setDuration - Gets the duration of the
     * stream and saves it in HH:mm:ss.SSS format to
     * duration. Emits change to QML
     * @param duration_qtime
     */
    void setDuration(QTime *duration_qtime);

    /**
     * @brief setPosition Sets the local variable; then
     * starts chain of events to then update the UI.
     * @param position_qtime
     */
    void setPosition(QTime position_qtime);


    MediaInfoGatherer media_info_gatherer;

    QString duration;
    QString position;

    QTimer position_timer;

    QGst::PipelinePtr m_pipeline;
    QGst::ElementPtr m_videoSink;
    QString m_baseDir;
};

#endif // PLAYER_H
