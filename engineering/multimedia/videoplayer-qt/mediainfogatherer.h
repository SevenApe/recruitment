#ifndef MEDIAINFOGATHERER_H
#define MEDIAINFOGATHERER_H

#include <QObject>
#include <QTime>

#include <memory>

#include <QGst/discoverer.h>
#include <QGst/pipeline.h>
#include <QGst/query.h>

using std::shared_ptr;

class MediaInfoGatherer
{
public:
    MediaInfoGatherer();

    /**
     * @brief getInfo Gathers common information from
     * the stream and makes it available in the class.
     * @param pipeline - Pipline to interrogate
     * @return
     */
    bool getInfo(QGst::PipelinePtr pipeline);

    /**
     * @brief getDuration in QTime format.
     * @return
     */
    QTime *getDuration();

    /**
     * @brief getPosition - Current time location
     * in the track
     * @return
     */
    QTime getPosition(QGst::PipelinePtr pipeline);

    /**
     * @brief info_loaded
     * True if the info has been loaded
     */
    bool info_loaded;
private:
   QTime duration;

};

#endif // MEDIAINFOGATHERER_H
