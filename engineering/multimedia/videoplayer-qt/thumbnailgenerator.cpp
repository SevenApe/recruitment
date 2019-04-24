#include "thumbnailgenerator.h"

#include <QImage>
#include <QDir>
#include <QDebug>
#include <QThread>

#include <QGst/Parse>
#include <QGst/bin.h>
#include <QGst/Sample>
#include <QGst/Buffer>
#include <QGst/ClockTime>
#include <QGst/ElementFactory>
#include <QGst/Event>
#include <QGst/Bus>
#include <QGlib/Connect>


class ThumbnailSink : public QGst::Utils::ApplicationSink
{
public:
    ThumbnailSink()
        : QGst::Utils::ApplicationSink() {}

protected:
    virtual void eos()
    {

    }

    virtual QGst::FlowReturn newSample()
    {
        QGst::SamplePtr sample = pullSample();
        return QGst::FlowOk;
    }

};

ThumbnailGenerator::ThumbnailGenerator(QObject *parent) : QThread(parent)
{

}

void ThumbnailGenerator::setUri(QString uri)
{
    this->uri = uri;
}

void ThumbnailGenerator::run()
{
    ThumbnailSink thumbnail_sink;

    // Set the pipeline description
    QString pipeline_description = "uridecodebin uri=" + uri + \
            " ! videoconvert ! videoscale ! appsink name=sink" + \
            " caps=\"video/x-raw,format=RGB,width=80,pixel-aspect-ratio=1/1\"";

    pipeline = QGst::Parse::launch(pipeline_description).dynamicCast<QGst::Pipeline>();
    // Set the sink
    sink = pipeline->getElementByName("sink");
    thumbnail_sink.setElement(pipeline->getElementByName("sink"));

    // Set the pipeline to the paused state to get the first frame in the sink
    QGst::StateChangeReturn state_ret = pipeline->setState(QGst::StatePaused);

    // This gives it time to change state
    state_ret = pipeline->getState(NULL, NULL, QGst::ClockTime().fromSeconds(5));

    // Get the duration
    media_info.getInfo(pipeline);
    QTime *duration = media_info.getDuration();

    // Calculate the positions for each of the thumbnails
    double duration_seconds = QTime(0, 0, 0).secsTo(*duration);
    double interval_seconds = duration_seconds / 10.0;

    QTime interval(0,0,0);

    for(int x=0; x<10; x++) {

        QGst::SamplePtr sample;
        sample = thumbnail_sink.pullPreroll();

        QGst::BufferPtr buffer;
        QGst::StructurePtr structure;
        QGst::CapsPtr caps = sample->caps();

        // Now we need to get the dimensions of the capture todo
        structure = caps->internalStructure(0);

        int width =  structure.data()->value("width").get<int>();
        int height = structure.data()->value("height").get<int>();

        QGst::MapInfo mapInfo;
        buffer = sample->buffer();
        buffer->map(mapInfo, QGst::MapRead);

        QImage new_thumbnail = QImage((const uchar *)mapInfo.data(),
                               width,
                               height,
                               QImage::Format_RGB888);

        image_provider.thumbnails[x] = new_thumbnail;

        // TODO bette to have an array of poniters to the signals
        switch(x) {
        case 0:
            Q_EMIT updateThumb0();
            break;
        case 1:
            Q_EMIT updateThumb1();
            break;
        case 2:
            Q_EMIT updateThumb2();
            break;
        case 3:
            Q_EMIT updateThumb3();
            break;
        case 4:
            Q_EMIT updateThumb4();
            break;
        case 5:
            Q_EMIT updateThumb5();
            break;
        case 6:
            Q_EMIT updateThumb6();
            break;
        case 7:
            Q_EMIT updateThumb7();
            break;
        case 8:
            Q_EMIT updateThumb8();
            break;
        case 9:
            Q_EMIT updateThumb9();
            break;
        }


        // Seek to the next 10th of the stream
        interval = interval.addSecs(interval_seconds);
        QGst::SeekEventPtr evt = QGst::SeekEvent::create(
            1.0, QGst::FormatTime, QGst::SeekFlagFlush,
            QGst::SeekTypeSet, QGst::ClockTime::fromTime(interval),
            QGst::SeekTypeNone, QGst::ClockTime::None
        );
        pipeline->sendEvent(evt);

        // Bit hacky here, is there a bus message that could be used?
        this->sleep(1);
    }

}

