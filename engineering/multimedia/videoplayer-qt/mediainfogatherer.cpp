#include "mediainfogatherer.h"

using std::make_shared;

MediaInfoGatherer::MediaInfoGatherer()
{
    info_loaded =false;
}

bool MediaInfoGatherer::getInfo(QGst::PipelinePtr pipeline)
{
    bool ret = false;

    QGst::DurationQueryPtr query = QGst::DurationQuery::create(QGst::FormatTime);
    pipeline->query(query);

    duration = QGst::ClockTime(query->duration()).toTime();

    info_loaded = true;

    return ret;
}

QTime *MediaInfoGatherer::getDuration()
{
    return &duration;
}

QTime MediaInfoGatherer::getPosition(QGst::PipelinePtr pipeline)
{
    QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
    pipeline->query(query);
    return QGst::ClockTime(query->position()).toTime();
}
