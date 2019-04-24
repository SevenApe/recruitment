#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QtCore>
#include <vector>
#include <QImage>

#include <QGst/Pipeline>
#include <QGst/Message>
#include <QGst/Utils/ApplicationSink>

#include "mediainfogatherer.h"
#include "thumbnailimageprovider.h"

using std::vector;

class ThumbnailGenerator : public QThread
{
    Q_OBJECT

public:
    explicit ThumbnailGenerator(QObject *parent = 0);
    virtual ~ThumbnailGenerator() {}

    /**
     * @brief setUri
     * @param uri - File path of video to play
     */
    void setUri(QString uri);

    /**
     * @brief image_provider Provides the thumbnails
     * to the QML layer
     */
    ThumbnailImageProvider image_provider;


Q_SIGNALS:
    /**
     * @brief These let the QML know the image is ready
     */
    void updateThumb0();
    void updateThumb1();
    void updateThumb2();
    void updateThumb3();
    void updateThumb4();
    void updateThumb5();
    void updateThumb6();
    void updateThumb7();
    void updateThumb8();
    void updateThumb9();


private:

    void run();

    void onBusMessage(const QGst::MessagePtr & message);

    MediaInfoGatherer media_info;

    QGst::PipelinePtr pipeline;
    QGst::ElementPtr sink;

    QString uri;
};

#endif // THUMBNAILGENERATOR_H
