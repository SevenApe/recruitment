#include "thumbnailimageprovider.h"


ThumbnailImageProvider::ThumbnailImageProvider() :
     QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
    thumbnails.resize(10);
}

QImage ThumbnailImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
    int id_it = id.toInt();

    return thumbnails[id_it];
}
