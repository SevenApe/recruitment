#ifndef THUMBNAILIMAGEPROVIDER_H
#define THUMBNAILIMAGEPROVIDER_H

#include <vector>

#include <QObject>
#include <QDeclarativeImageProvider>
#include <QImage>

using std::vector;

class ThumbnailImageProvider : public QDeclarativeImageProvider
{
public:
    ThumbnailImageProvider();

    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);

    vector<QImage> thumbnails;
};

#endif // THUMBNAILIMAGEPROVIDER_H
