#include "tilecachehd.h"
#include "app.h"
#include <QFile>
#include <QDir>
#include <QPainter>

typedef float FLOAT;

class Vector2F
{
public:

    Vector2F()
    {

    }

    Vector2F(FLOAT const r, FLOAT const g)
    {
        _data[0] = r;
        _data[1] = g;
    }

    Vector2F(std::initializer_list<FLOAT> l)
    {
        *this = l;
    }

    FLOAT & operator[](int const i)
    {
        return _data[i];
    }

    FLOAT const & operator[](int const i) const
    {
        return _data[i];
    }

    void operator=(std::initializer_list<FLOAT> l)
    {
        if (l.size() != 2)
        {
            qWarning() << "Invalid initializer size";
            return;
        }

        _data[0] = l.begin()[0];
        _data[1] = l.begin()[1];
    }

    bool operator==(Vector2F const & other)
    {
        for (int i = 0; i != 2; ++i)
            if (_data[i] != other._data[i])
                return false;
        return true;
    }

    Vector2F & operator+=(Vector2F const & other)
    {
        _data[0] += other._data[0];
        _data[1] += other._data[1];
        return *this;
    }

    Vector2F & operator-=(Vector2F const & other)
    {
        _data[0] -= other._data[0];
        _data[1] -= other._data[1];
        return *this;
    }

    Vector2F & operator*=(FLOAT const value)
    {
        _data[0] *= value;
        _data[1] *= value;
        return *this;
    }

    Vector2F & operator/=(FLOAT const value)
    {
        _data[0] /= value;
        _data[1] /= value;
        return *this;
    }

    Vector2F operator+(FLOAT const value) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] + value;
        tmp._data[1] = _data[1] + value;
        return tmp;
    }

    Vector2F operator-(FLOAT const value) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] - value;
        tmp._data[1] = _data[1] - value;
        return tmp;
    }

    Vector2F operator*(FLOAT const value) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] * value;
        tmp._data[1] = _data[1] * value;
        return tmp;
    }

    Vector2F operator/(FLOAT const value) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] / value;
        tmp._data[1] = _data[1] / value;
        return tmp;
    }

    Vector2F operator+(Vector2F const & other) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] + other._data[0];
        tmp._data[1] = _data[1] + other._data[1];
        return tmp;
    }

    Vector2F operator-(Vector2F const & other) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] - other._data[0];
        tmp._data[1] = _data[1] - other._data[1];
        return tmp;
    }

    Vector2F operator*(Vector2F const & value) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] * value[0];
        tmp._data[1] = _data[1] * value[1];
        return tmp;
    }

    Vector2F operator/(Vector2F const & value) const
    {
        Vector2F tmp;
        tmp._data[0] = _data[0] / value[0];
        tmp._data[1] = _data[1] / value[1];
        return tmp;
    }

    FLOAT module2() const
    {
        FLOAT res = 0.0;
        res += _data[0] * _data[0];
        res += _data[1] * _data[1];
        return res;
    }

    FLOAT module() const
    {
        return std::sqrt(module2());
    }

    Vector2F normalize() const
    {
        Vector2F out;
        FLOAT const d = module();
        out[0] = _data[0] / d;
        out[1] = _data[1] / d;
        return out;
    }

    Vector2F flip() const
    {
        return Vector2F(_data[1], _data[0]);
    }

private:

    FLOAT _data[2];

};

std::ostream & operator<<(std::ostream & o, Vector2F const & c)
{
    o << "(" << c[0] << "," << c[1] << ")";
    return o;
}

class Vector3F
{
public:

    Vector3F()
    {

    }

    Vector3F(FLOAT const r, FLOAT const g, FLOAT const b)
    {
        _data[0] = r;
        _data[1] = g;
        _data[2] = b;
    }

    Vector3F(std::initializer_list<FLOAT> l)
    {
        *this = l;
    }

    FLOAT & operator[](int const i)
    {
        return _data[i];
    }

    FLOAT const & operator[](int const i) const
    {
        return _data[i];
    }

    void operator=(std::initializer_list<FLOAT> l)
    {
        if (l.size() != 3)
        {
            qWarning() << "Invalid initializer size";
            return;
        }

        _data[0] = l.begin()[0];
        _data[1] = l.begin()[1];
        _data[2] = l.begin()[2];
    }

    bool operator==(Vector3F const & other)
    {
        for (int i = 0; i != 3; ++i)
            if (_data[i] != other._data[i])
                return false;
        return true;
    }

    Vector3F & operator+=(Vector3F const & other)
    {
        _data[0] += other._data[0];
        _data[1] += other._data[1];
        _data[2] += other._data[2];
        return *this;
    }

    Vector3F & operator-=(Vector3F const & other)
    {
        _data[0] -= other._data[0];
        _data[1] -= other._data[1];
        _data[2] -= other._data[2];
        return *this;
    }

    Vector3F & operator*=(FLOAT const value)
    {
        _data[0] *= value;
        _data[1] *= value;
        _data[2] *= value;
        return *this;
    }

    Vector3F & operator/=(FLOAT const value)
    {
        _data[0] /= value;
        _data[1] /= value;
        _data[2] /= value;
        return *this;
    }

    Vector3F operator+(FLOAT const value) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] + value;
        tmp._data[1] = _data[1] + value;
        tmp._data[2] = _data[2] + value;
        return tmp;
    }

    Vector3F operator-(FLOAT const value) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] - value;
        tmp._data[1] = _data[1] - value;
        tmp._data[2] = _data[2] - value;
        return tmp;
    }

    Vector3F operator*(FLOAT const value) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] * value;
        tmp._data[1] = _data[1] * value;
        tmp._data[2] = _data[2] * value;
        return tmp;
    }

    Vector3F operator/(FLOAT const value) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] / value;
        tmp._data[1] = _data[1] / value;
        tmp._data[2] = _data[2] / value;
        return tmp;
    }

    Vector3F operator+(Vector3F const & other) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] + other._data[0];
        tmp._data[1] = _data[1] + other._data[1];
        tmp._data[2] = _data[2] + other._data[2];
        return tmp;
    }

    Vector3F operator-(Vector3F const & other) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] - other._data[0];
        tmp._data[1] = _data[1] - other._data[1];
        tmp._data[2] = _data[2] - other._data[2];
        return tmp;
    }

    Vector3F operator*(Vector3F const & value) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] * value[0];
        tmp._data[1] = _data[1] * value[1];
        tmp._data[2] = _data[2] * value[2];
        return tmp;
    }

    Vector3F operator/(Vector3F const & value) const
    {
        Vector3F tmp;
        tmp._data[0] = _data[0] / value[0];
        tmp._data[1] = _data[1] / value[1];
        tmp._data[2] = _data[2] / value[2];
        return tmp;
    }

    FLOAT module2() const
    {
        FLOAT res = 0.0;
        res += _data[0] * _data[0];
        res += _data[1] * _data[1];
        res += _data[2] * _data[2];
        return res;
    }

    FLOAT module() const
    {
        return std::sqrt(module2());
    }

    Vector3F normalize() const
    {
        Vector3F out;
        FLOAT const d = module();
        out[0] = _data[0] / d;
        out[1] = _data[1] / d;
        out[2] = _data[2] / d;
        return out;
    }

    Vector3F flip() const
    {
        return Vector3F(_data[2], _data[1], _data[0]);
    }

    Vector2F sub0() const
    {
        return Vector2F(_data[0], _data[1]);
    }

    Vector2F sub1() const
    {
        return Vector2F(_data[1], _data[2]);
    }

    Vector2F sub2() const
    {
        return Vector2F(_data[2], _data[0]);
    }

private:

    FLOAT _data[3];

};

std::ostream & operator<<(std::ostream & o, Vector3F const & c)
{
    o << "(" << c[0] << "," << c[1] << "," << c[2] << ")";
    return o;
}

#define XYZReferenceX 100
#define XYZReferenceY 100
#define XYZReferenceZ 100

inline Vector3F rgb2hsv(Vector3F const & in)
{
    auto var_R = ( in[0] / 255.0 );
    auto var_G = ( in[1] / 255.0 );
    auto var_B = ( in[2] / 255.0 );

    auto var_Min = std::min( var_R, std::min(var_G, var_B) );    // Min. value of RGB
    auto var_Max = std::max( var_R, std::min(var_G, var_B) );    // Max. value of RGB
    auto del_Max = var_Max - var_Min;                        // Delta RGB value

    Vector3F out;

    out[2] = var_Max;

    if ( del_Max == 0 ) // This is a gray, no chroma...
    {
        out[0] = 0;
        out[1] = 0;
    }
    else // Chromatic data...
    {
        out[1] = del_Max / var_Max;

        auto del_R = ( ( ( var_Max - var_R ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
        auto del_G = ( ( ( var_Max - var_G ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
        auto del_B = ( ( ( var_Max - var_B ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;

        if      ( var_R == var_Max ) out[0] = del_B - del_G;
        else if ( var_G == var_Max ) out[0] = ( 1.0 / 3.0 ) + del_R - del_B;
        else if ( var_B == var_Max ) out[0] = ( 2.0 / 3.0 ) + del_G - del_R;

        if ( out[0] < 0.0 ) out[0] += 1.0;
        if ( out[0] > 1.0 ) out[0] -= 1.0;
    }

    return out;
}

inline Vector3F hsv2rgb(Vector3F const & in)
{
    if ( in[1] == 0 )
        return Vector3F(in[2] * 255, in[2] * 255, in[2] * 255);

    auto var_h = in[0] * 6.0;
    if ( var_h == 6.0 ) var_h = 0.0;      //H must be < 1
    auto var_i = int( var_h );             //Or ... var_i = floor( var_h )

    #define var_1 in[2] * ( 1 - in[1] );
    #define var_2 in[2] * ( 1 - in[1] * ( var_h - var_i ) );
    #define var_3 in[2] * ( 1 - in[1] * ( 1 - ( var_h - var_i ) ) );

    FLOAT var_r, var_g, var_b;

    if      ( var_i == 0 ) { var_r = in[2] ; var_g = var_3 ; var_b = var_1; }
    else if ( var_i == 1 ) { var_r = var_2 ; var_g = in[2] ; var_b = var_1; }
    else if ( var_i == 2 ) { var_r = var_1 ; var_g = in[2] ; var_b = var_3; }
    else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = in[2]; }
    else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = in[2]; }
    else                   { var_r = in[2] ; var_g = var_1 ; var_b = var_2; }

    return Vector3F( var_r * 255, var_g * 255, var_b * 255);
}

inline Vector3F rgb2xyz(Vector3F const & in)
{
    auto var_R = ( in[0] / 255.0 );
    auto var_G = ( in[1] / 255.0 );
    auto var_B = ( in[2] / 255.0 );

    if ( var_R > 0.04045 ) var_R = std::pow(( ( var_R + 0.055 ) / 1.055 ), 2.4);
    else var_R = var_R / 12.92;

    if ( var_G > 0.04045 ) var_G = std::pow(( ( var_G + 0.055 ) / 1.055 ), 2.4);
    else var_G = var_G / 12.92;

    if ( var_B > 0.04045 ) var_B = std::pow(( ( var_B + 0.055 ) / 1.055 ), 2.4);
    else var_B = var_B / 12.92;

    var_R = var_R * 100.0;
    var_G = var_G * 100.0;
    var_B = var_B * 100.0;

    return Vector3F(
        var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805,
        var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722,
        var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505);
}

inline Vector3F xyz2rgb(Vector3F const & in)
{
    auto var_X = in[0] / 100.0;
    auto var_Y = in[1] / 100.0;
    auto var_Z = in[2] / 100.0;

    auto var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
    auto var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
    auto var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

    if ( var_R > 0.0031308 ) var_R = 1.055 * ( std::pow(var_R, ( 1 / 2.4 )) ) - 0.055;
    else                     var_R = 12.92 * var_R;

    if ( var_G > 0.0031308 ) var_G = 1.055 * ( std::pow(var_G, ( 1 / 2.4 )) ) - 0.055;
    else                     var_G = 12.92 * var_G;

    if ( var_B > 0.0031308 ) var_B = 1.055 * ( std::pow(var_B, ( 1 / 2.4 )) ) - 0.055;
    else                     var_B = 12.92 * var_B;

    return Vector3F( var_R * 255, var_G * 255, var_B * 255);
}

inline Vector3F xyz2lab(Vector3F const & in)
{
    auto var_X = in[0] / XYZReferenceX;
    auto var_Y = in[1] / XYZReferenceY;
    auto var_Z = in[2] / XYZReferenceZ;

    if ( var_X > 0.008856 ) var_X = std::pow(var_X, ( 1.0/3.0 ));
    else                    var_X = ( 7.787 * var_X ) + ( 16.0 / 116.0 );

    if ( var_Y > 0.008856 ) var_Y = std::pow(var_Y, ( 1.0/3.0 ));
    else                    var_Y = ( 7.787 * var_Y ) + ( 16.0 / 116.0 );

    if ( var_Z > 0.008856 ) var_Z = std::pow(var_Z, ( 1.0/3.0 ));
    else                    var_Z = ( 7.787 * var_Z ) + ( 16.0 / 116.0 );

    return Vector3F(
        ( 116.0 * var_Y ) - 16.0,
        500.0 * ( var_X - var_Y ),
        200.0 * ( var_Y - var_Z ));
}

inline Vector3F lab2xyz(Vector3F const & in)
{
    auto var_Y = ( in[0] + 16.0 ) / 116.0;
    auto var_X = in[1] / 500.0 + var_Y;
    auto var_Z = var_Y - in[2] / 200.0;

    auto var_Y_3 = std::pow(var_Y,3.0);
    auto var_X_3 = std::pow(var_X,3.0);
    auto var_Z_3 = std::pow(var_Z,3.0);

    if ( var_X_3  > 0.008856 ) var_X = var_X_3;
    else                       var_X = ( var_X - 16.0 / 116.0 ) / 7.787;

    if ( var_Y_3  > 0.008856 ) var_Y = var_Y_3;
    else                       var_Y = ( var_Y - 16.0 / 116.0 ) / 7.787;

    if ( var_Z_3  > 0.008856 ) var_Z = var_Z_3;
    else                       var_Z = ( var_Z - 16.0 / 116.0 ) / 7.787;

    return Vector3F(
        var_X * XYZReferenceX,
        var_Y * XYZReferenceY,
        var_Z * XYZReferenceZ);
}

inline Vector3F rgb2lab(Vector3F const & in)
{
    return xyz2lab(rgb2xyz(in));
}

inline Vector3F lab2rgb(Vector3F const & in)
{
    return xyz2rgb(lab2xyz(in));
}

#define CLIP255(x) ((x)<0?0:(x)>255?255:(x))

inline Vector3F clipRGB(Vector3F const & in)
{
    Vector3F out;
    out[0] = CLIP255(in[0]);
    out[1] = CLIP255(in[1]);
    out[2] = CLIP255(in[2]);
    return out;
}

class QImageWrapper
{
public:

    QImageWrapper(QImage & img, bool hFlip, bool vFlip) :
        _img(img),
        _hFlip(hFlip),
        _vFlip(vFlip)
    {

    }

    Vector3F getPixel(size_t const i, size_t const j) const
    {
        auto values = _img.pixel(j,i);
        Vector3F tmp;
        tmp[0] = qRed(values);
        tmp[1] = qGreen(values);
        tmp[2] = qBlue(values);
        return tmp;
    }

    void setPixel(size_t i, size_t j, Vector3F const & color) const
    {
        auto const values = qRgba(CLIP255(color[0]), CLIP255(color[1]), CLIP255(color[2]), 255);
        if (_vFlip) i = _img.height()-i-1;
        if (_hFlip) j = _img.width()-j-1;
        _img.setPixel(j,i,values);
    }

    size_t rows() const
    {
        return _img.height();
    }

    size_t cols() const
    {
        return _img.width();
    }

private:

    QImage & _img;
    bool _hFlip;
    bool _vFlip;

};

template <typename IMAGE_WRAPPER>
inline void colorTransform12(std::vector<Vector3F> const & colors1,
                             std::vector<Vector3F> const & colors2,
                             IMAGE_WRAPPER const & imgIn,
                             IMAGE_WRAPPER & imgOut)
{
    size_t const k = std::min(colors1.size(), colors2.size());

    std::vector<Vector3F> labs1;
    std::vector<Vector3F> labs2;

    for (size_t i=0;i!=k;++i)
        labs1.push_back(rgb2lab(colors1[i]));

    for (size_t i=0;i!=k;++i)
        labs2.push_back(rgb2lab(colors2[i]));

    for (size_t i = 0; i != imgIn.rows(); ++i)
    {
        for (size_t j = 0; j != imgIn.cols(); ++j)
        {
            Vector3F rgbIn = imgIn.getPixel(i, j);
            Vector3F labIn = rgb2lab(rgbIn);

            FLOAT bestDistance1;
            FLOAT bestDistance2;
            FLOAT bestDistance3;
            size_t bestP1 = k;
            size_t bestP2 = k;
            size_t bestP3 = k;

            for (size_t p = 0; p != k; ++p)
            {
                FLOAT const d = (labIn-labs1[p]).module();

                if (bestP1 == k || d < bestDistance1)
                {
                    bestDistance3 = bestDistance2;
                    bestDistance2 = bestDistance1;
                    bestDistance1 = d;

                    bestP3 = bestP2;
                    bestP2 = bestP1;
                    bestP1 = p;
                }
                else if (bestP2 == k || d < bestDistance2)
                {
                    bestDistance3 = bestDistance2;
                    bestDistance2 = d;

                    bestP3 = bestP2;
                    bestP2 = p;
                }
                else if (bestP3 == k || d < bestDistance3)
                {
                    bestDistance3 = d;

                    bestP3 = p;
                }
            }

            Vector3F labOut {0,0,0};

            if (bestP1 != k && bestP2 != k && bestP3 != k)
            {
                auto const w1 = 1/(bestDistance1+1);
                auto const w2 = 1/(bestDistance2+1);
                auto const w3 = 1/(bestDistance3+1);

                labOut = labIn + ((labs2[bestP1]-labs1[bestP1]) * w1 +
                                  (labs2[bestP2]-labs1[bestP2]) * w2 +
                                  (labs2[bestP3]-labs1[bestP3]) * w3) / (w1 + w2 + w3);
            }
            else if (bestP1 != k && bestP2 != k)
            {
                auto const w1 = 1/(bestDistance1+1);
                auto const w2 = 1/(bestDistance2+1);

                labOut = labIn + ((labs2[bestP1]-labs1[bestP1]) * w1 +
                                  (labs2[bestP2]-labs1[bestP2]) * w2) / (w1 + w2);
            }

            Vector3F rgbOut = lab2rgb(labOut);
            imgOut.setPixel(i, j, clipRGB(rgbOut));
        }
    }
}

TileCacheHD::TileCacheHD()
{

}

void TileCacheHD::clear()
{
    for (auto pair : _cachedImages.asKeyValueRange())
        delete pair.second;

    for (auto pair : _cachedPixmaps.asKeyValueRange())
        delete pair.second;

    for (auto pair : _tileID2tileFile.asKeyValueRange())
        delete pair.second;

    _cachedImages.clear();
    _cachedPixmaps.clear();
    _tileID2tileFile.clear();
}

void TileCacheHD::updateKnownTiles(QString const & projectPath)
{
#define ABORT(msg) {\
        qWarning() << "Could not parse HD Tiles: " << msg;\
        return;\
    }

#define SKIP(msg) {\
        qWarning() << "Could not parse HD Tile: " << msg;\
        continue;\
    }

    QDir tilesDir(projectPath);
    char const * tilesFoldername = "tiles.high";

    if (!tilesDir.exists(tilesFoldername)) ABORT("Tiles folder does not exists");
    if (!tilesDir.cd(tilesFoldername)) ABORT("Could not cd to tiles folder");

    bool ok;

    for (auto & filename : tilesDir.entryList(QDir::Files))
    {
        auto basename = QFileInfo(filename).baseName();
        auto cells = basename.split("_");

        if (cells.size() != 5)
        {
            qWarning() << "Invalid tile file:" << basename;
            continue;
        }

        int32_t tileID = cells[0].toInt(&ok);
        if (!ok) SKIP(QString("Invalid tileID value in filename: %1.").arg(filename));

        int32_t paletteID = cells[1].toInt(&ok);
        if (!ok) SKIP(QString("Invalid paletteID value in filename: %1.").arg(filename));

        int hFlip = cells[2].toInt(&ok);
        if (!ok) SKIP(QString("Invalid hFlip value in filename: %1.").arg(filename));

        int vFlip = cells[3].toInt(&ok);
        if (!ok) SKIP(QString("Invalid vFlip value in filename: %1.").arg(filename));

        int tilesetID = cells[4].toInt(&ok);
        if (!ok) SKIP(QString("Invalid tilesetID value in filename: %1.").arg(filename));

        auto tileFile = new TileFile();
        tileFile->filename = filename;
        tileFile->tileID = tileID;
        tileFile->paletteID = paletteID;
        tileFile->tilesetID = tilesetID;
        tileFile->hFlip = hFlip;
        tileFile->vFlip = vFlip;
        _tileID2tileFile.emplace(tileID, tileFile);
    }
    qWarning() << "Found " << _tileID2tileFile.size() << " tiles";
#undef ABORT
#undef SKIP
}

QImage * TileCacheHD::loadImage(Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
#define ABORT(msg) {\
        qWarning() << "Could not load HD Tile: " << msg;\
        return nullptr;\
    }

    // TODO: Load mask and add transparency

    auto project = App::getState()->project();
    if (project == nullptr) ABORT("Project is nullptr");

    QDir tilesDir(project->path);
    char const * tilesFoldername = "tiles.high";

    if (!tilesDir.exists(tilesFoldername)) ABORT("Tiles folder does not exists");
    if (!tilesDir.cd(tilesFoldername)) ABORT("Could not cd to tiles folder");

    auto it = _tileID2tileFile.constFind(tile->id);
    if (it == _tileID2tileFile.constEnd()) ABORT(QString("Could not find original tile file for id %1").arg(tile->id));

    auto tileFile = it.value();
    auto paletteOld = App::getState()->getPaletteById(tileFile->paletteID);
    if (paletteOld == nullptr) ABORT("Could not find the original palette");

    QImage imgIn;

    if (!imgIn.load(tilesDir.filePath(tileFile->filename))) ABORT("Could not load original tile file");

    QImage * imgOut = new QImage(imgIn.width(), imgIn.height(), QImage::Format_ARGB32);

    QImageWrapper inputWrapper(imgIn, false, false);
    QImageWrapper outputWrapper(*imgOut, hFlip ^ tileFile->hFlip, vFlip ^ tileFile->vFlip);

    std::vector<Vector3F> colors1;
    std::vector<Vector3F> colors2;

    for (qsizetype i=1;i!=paletteOld->size;++i)
    {
        QColor & c = paletteOld->colors[i];
        colors1.push_back(Vector3F(c.red(), c.green(), c.blue()));
    }

    for (qsizetype i=1;i!=palette->size;++i)
    {
        QColor & c = palette->colors[i];
        colors2.push_back(Vector3F(c.red(), c.green(), c.blue()));
    }

    if (tile->id == 386)
        qWarning() << "Found it!";

    colorTransform12(colors1, colors2, inputWrapper, outputWrapper);

    qWarning() << "Loaded tile for tileID=" << tile->id << " paletteID=" << palette->id << " hFlip=" << hFlip << " vFlip=" << vFlip;

    return imgOut;
#undef ABORT
}

void TileCacheHD::createCache(QPair<QPair<int,int>,QPair<bool,bool>> & key,
                              Tile * tile, Palette * palette, bool hFlip, bool vFlip)
{
    QImage * img = loadImage(tile, palette, hFlip, vFlip);

    _cachedImages[key] = img == nullptr ? nullptr : img;
    _cachedPixmaps[key] = img == nullptr ? nullptr : new QPixmap(QPixmap::fromImage(*img));
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
