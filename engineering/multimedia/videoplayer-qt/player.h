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
#include <QGst/Pipeline>
#include <QGst/Message>

#include "mediainfogatherer.h"

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString duration READ getDuration NOTIFY durationUpdated)
public:
    explicit Player(QObject *parent = 0);
    void setVideoSink(const QGst::ElementPtr & sink);
    QString getDuration();

public Q_SLOTS:
    void play();
    void stop();
    void open();

Q_SIGNALS:
    void durationUpdated();

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

    MediaInfoGatherer media_info_gatherer;

    QString duration;

    QGst::PipelinePtr m_pipeline;
    QGst::ElementPtr m_videoSink;
    QString m_baseDir;
};

#endif // PLAYER_H
