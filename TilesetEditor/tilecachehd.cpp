#include "tilecachehd.h"
#include "app.h"
#include <QFile>
#include <QDir>
#include <QPainter>

TileCacheHD::TileCacheHD()
{

}

void TileCacheHD::clear()
{
    for (auto pair : _cachedImages.asKeyValueRange())
        delete pair.second;

    for (auto pair : _cachedPixmaps.asKeyValueRange())
        delete pair.second;

    _cachedImages.clear();
    _cachedPixmaps.clear();
}

QImage * TileCacheHD::loadImage(Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
#define ABORT(msg) {\
        qWarning() << "Could not load HD Tile: " << msg;\
        return nullptr;\
    }

//    if (img.width() != 128 || img.height() != 128 || img.format() != QImage::Format_ARGB32)
//        ABORT("Invalid image format or size for TileCacheHD::loadImage. Expected 128x128 Format_ARGB32.");

    auto project = App::getState()->project();
    if (project == nullptr) ABORT("Project is nullptr");

    auto tinyImg = App::getOriginalTileCache()->getTileImage(tile, palette, hFlip, vFlip);
    if (tinyImg == nullptr) ABORT("Reference tinyImg could not be loaded");

    QDir encodedDir(project->path);
    char const * encodedFoldername = "encoded.high";

    if (!encodedDir.exists(encodedFoldername)) ABORT("Encoded folder does not exists");
    if (!encodedDir.cd(encodedFoldername)) ABORT("Could not cd to encoded folder");

    QString filename {QString("%1.bin").arg(tile->id)};
    QFile file(encodedDir.filePath(filename));
    if (!file.open(QIODevice::ReadOnly)) ABORT(QString("File %1 could not be open.").arg(filename));

    QByteArray data = file.readAll();

    if (data.size() < (qsizetype)(3*sizeof(int))) ABORT(QString("File %1 has an invalid size.").arg(filename));

    int const width = ((int*)data.data())[0];
    int const height = ((int*)data.data())[1];
    int const bgColor = ((int*)data.data())[2];

    if (width % 8 != 0) ABORT(QString("Invalid hd tile in %1. A tile width must be divisible by 8.").arg(filename));
    if (height % 8 != 0) ABORT(QString("Invalid hd tile in %1. A tile height must be divisible by 8.").arg(filename));

    int const gridWidth = width / 8;
    int const gridHeight = height / 8;

    if (data.size() != static_cast<qsizetype>(3*sizeof(int)+width*height*4)) ABORT(QString("File %1 has an invalid size.").arg(filename));

    QImage * img = new QImage(width, height, QImage::Format_ARGB32);
    int k = 3*sizeof(int);
    int i0,i1,iStep;
    int j0,j1,jStep;

    if (vFlip) { i0 = height-1; i1 = -1    ; iStep = -1; }
    else       { i0 = 0       ; i1 = height; iStep = +1; }

    if (hFlip) { j0 = width-1; j1 = -1   ; jStep = -1; }
    else       { j0 = 0      ; j1 = width; jStep = +1; }

    QRgb newColor;

    for (int i=i0;i!=i1;i+=iStep)
    {
        float const y = float(i) / float(gridHeight);
        int const y1 = y;
        int const y2 = tinyImg->height()-1==y1?y1:y1+1;
        float const yf = y - y1;

        for (int j=j0;j!=j1;j+=jStep)
        {
            float const x = float(j) / float(gridWidth);
            int const x1 = x;
            int const x2 = tinyImg->width()-1==x1 ? x1 : x1+1;
            float const xf = x - x1;

            if (data[k] == 0)
            {
                newColor = qRgba(0,0,0,0);
            }
            else
            {
                QRgb A = tinyImg->pixel(x1,y1);
                QRgb B = tinyImg->pixel(x2,y1);
                QRgb C = tinyImg->pixel(x1,y2);
                QRgb D = tinyImg->pixel(x2,y2);

                if (qAlpha(A) == 0) A = bgColor;
                if (qAlpha(B) == 0) B = bgColor;
                if (qAlpha(C) == 0) C = bgColor;
                if (qAlpha(D) == 0) D = bgColor;

                QRgb refColor = MERGE_COLORS(MERGE_COLORS(A,B,xf),MERGE_COLORS(C,D,xf),yf);

                newColor = decodeHDColor((uchar*)&data[k], refColor);

                if (tile->id == 240 && i==64 && j==64)
                {
                    qDebug() << "k:" << k;
                    qDebug() << "j i:" << j << i;
                    qDebug() << "x y:" << x << y;
                    qDebug() << "xf yf:" << xf << yf;
                    qDebug() << "x1 x2:" << x1 << x2;
                    qDebug() << "y1 y2:" << y1 << y2;
                    qDebug() << "A B C D:" << A << B << C << D;
                    qDebug() << "refColor:" << refColor;
                    qDebug() << "hdImgColor:" << newColor;

                    qWarning() << "w h hFlip vFlip" << width << height << vFlip << hFlip;
                    qWarning() << "iRange" << i0 << i1 << iStep;
                    qWarning() << "jRange" << j0 << j1 << jStep;
                }
            }

//            qWarning() << qRed(newColor) << qGreen(newColor) << qBlue(newColor) << qAlpha(newColor);
            img->setPixel(j, i, newColor);
            k += 4;
        }
    }

    return img;
}

void TileCacheHD::createCache(QPair<QPair<int,int>,QPair<bool,bool>> & key,
                              Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    QImage * img = loadImage(tile, palette, hFlip, vFlip);

    _cachedImages[key] = new QImage(*img);
    _cachedPixmaps[key] = new QPixmap(QPixmap::fromImage(*img));
}

QPixmap * TileCacheHD::getTilePixmap(Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    if (tile == nullptr || palette == nullptr)
        return nullptr;

    QPair<QPair<int,int>,QPair<bool,bool>> key(QPair<int,int>(tile->id, palette->id), QPair<bool,bool>(hFlip, vFlip));

    if (!_cachedPixmaps.contains(key))
        createCache(key, tile, palette, hFlip, vFlip);

    return _cachedPixmaps[key];
}
