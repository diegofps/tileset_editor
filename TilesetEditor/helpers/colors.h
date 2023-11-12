#ifndef COLORS_H
#define COLORS_H

#include "helpers/vectors.h"

#define XYZReferenceX FLOAT(100)
#define XYZReferenceY FLOAT(100)
#define XYZReferenceZ FLOAT(100)

inline Vector3F rgb2hsv(Vector3F const & in)
{
    FLOAT var_R = ( in[0] / FLOAT(255) );
    FLOAT var_G = ( in[1] / FLOAT(255) );
    FLOAT var_B = ( in[2] / FLOAT(255) );

    FLOAT var_Min = std::min( var_R, std::min(var_G, var_B) );    // Min. value of RGB
    FLOAT var_Max = std::max( var_R, std::min(var_G, var_B) );    // Max. value of RGB
    FLOAT del_Max = var_Max - var_Min;                        // Delta RGB value

    Vector3F out;

    out[2] = var_Max;

    if ( del_Max == FLOAT(0) ) // This is a gray, no chroma...
    {
        out[0] = FLOAT(0);
        out[1] = FLOAT(0);
    }
    else // Chromatic data...
    {
        out[1] = del_Max / var_Max;

        FLOAT del_R = ( ( ( var_Max - var_R ) / FLOAT(6) ) + ( del_Max / FLOAT(2) ) ) / del_Max;
        FLOAT del_G = ( ( ( var_Max - var_G ) / FLOAT(6) ) + ( del_Max / FLOAT(2) ) ) / del_Max;
        FLOAT del_B = ( ( ( var_Max - var_B ) / FLOAT(6) ) + ( del_Max / FLOAT(2) ) ) / del_Max;

        if      ( var_R == var_Max ) out[0] = del_B - del_G;
        else if ( var_G == var_Max ) out[0] = ( FLOAT(1) / FLOAT(3) ) + del_R - del_B;
        else if ( var_B == var_Max ) out[0] = ( FLOAT(2) / FLOAT(3) ) + del_G - del_R;

        if ( out[0] < FLOAT(0) ) out[0] += FLOAT(1);
        if ( out[0] > FLOAT(1) ) out[0] -= FLOAT(1);
    }

    return out;
}

inline Vector3F hsv2rgb(Vector3F const & in)
{
    if ( in[1] == FLOAT(0) )
        return Vector3F(in[2] * FLOAT(255), in[2] * FLOAT(255), in[2] * FLOAT(255));

    FLOAT var_h = in[0] * FLOAT(6);
    if ( var_h == FLOAT(6) ) var_h = FLOAT(0);      //H must be < 1
    int var_i = int( var_h );             //Or ... var_i = floor( var_h )

    #define var_1 in[2] * ( FLOAT(1) - in[1] );
    #define var_2 in[2] * ( FLOAT(1) - in[1] * ( var_h - var_i ) );
    #define var_3 in[2] * ( FLOAT(1) - in[1] * ( FLOAT(1) - ( var_h - var_i ) ) );

    FLOAT var_r, var_g, var_b;

    if      ( var_i == 0 ) { var_r = in[2] ; var_g = var_3 ; var_b = var_1; }
    else if ( var_i == 1 ) { var_r = var_2 ; var_g = in[2] ; var_b = var_1; }
    else if ( var_i == 2 ) { var_r = var_1 ; var_g = in[2] ; var_b = var_3; }
    else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = in[2]; }
    else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = in[2]; }
    else                   { var_r = in[2] ; var_g = var_1 ; var_b = var_2; }

    return Vector3F( var_r * FLOAT(255), var_g * FLOAT(255), var_b * FLOAT(255));
}

inline Vector3F rgb2xyz(Vector3F const & in)
{
    FLOAT var_R = ( in[0] / FLOAT(255) );
    FLOAT var_G = ( in[1] / FLOAT(255) );
    FLOAT var_B = ( in[2] / FLOAT(255) );

    if ( var_R > FLOAT(0.04045) ) var_R = std::pow(( ( var_R + FLOAT(0.055) ) / FLOAT(1.055) ), FLOAT(2.4));
    else var_R = var_R / FLOAT(12.92);

    if ( var_G > FLOAT(0.04045) ) var_G = std::pow(( ( var_G + FLOAT(0.055) ) / FLOAT(1.055) ), FLOAT(2.4));
    else var_G = var_G / FLOAT(12.92);

    if ( var_B > FLOAT(0.04045) ) var_B = std::pow(( ( var_B + FLOAT(0.055) ) / FLOAT(1.055) ), FLOAT(2.4));
    else var_B = var_B / FLOAT(12.92);

    var_R = var_R * FLOAT(100);
    var_G = var_G * FLOAT(100);
    var_B = var_B * FLOAT(100);

    return Vector3F(
        var_R * FLOAT(0.4124) + var_G * FLOAT(0.3576) + var_B * FLOAT(0.1805),
        var_R * FLOAT(0.2126) + var_G * FLOAT(0.7152) + var_B * FLOAT(0.0722),
        var_R * FLOAT(0.0193) + var_G * FLOAT(0.1192) + var_B * FLOAT(0.9505));
}

inline Vector3F xyz2rgb(Vector3F const & in)
{
    FLOAT var_X = in[0] / FLOAT(100);
    FLOAT var_Y = in[1] / FLOAT(100);
    FLOAT var_Z = in[2] / FLOAT(100);

    FLOAT var_R = var_X *  FLOAT(3.2406) + var_Y * -FLOAT(1.5372) + var_Z * -FLOAT(0.4986);
    FLOAT var_G = var_X * -FLOAT(0.9689) + var_Y *  FLOAT(1.8758) + var_Z *  FLOAT(0.0415);
    FLOAT var_B = var_X *  FLOAT(0.0557) + var_Y * -FLOAT(0.2040) + var_Z *  FLOAT(1.0570);

    if ( var_R > FLOAT(0.0031308) ) var_R = FLOAT(1.055) * ( std::pow(var_R, ( FLOAT(1) / FLOAT(2.4) )) ) - FLOAT(0.055);
    else                            var_R = FLOAT(12.92) * var_R;

    if ( var_G > FLOAT(0.0031308) ) var_G = FLOAT(1.055) * ( std::pow(var_G, ( FLOAT(1) / FLOAT(2.4) )) ) - FLOAT(0.055);
    else                            var_G = FLOAT(12.92) * var_G;

    if ( var_B > FLOAT(0.0031308) ) var_B = FLOAT(1.055) * ( std::pow(var_B, ( FLOAT(1) / FLOAT(2.4) )) ) - FLOAT(0.055);
    else                            var_B = FLOAT(12.92) * var_B;

    return Vector3F( var_R * FLOAT(255), var_G * FLOAT(255), var_B * FLOAT(255));
}

inline Vector3F xyz2lab(Vector3F const & in)
{
    FLOAT var_X = in[0] / XYZReferenceX;
    FLOAT var_Y = in[1] / XYZReferenceY;
    FLOAT var_Z = in[2] / XYZReferenceZ;

    if ( var_X > FLOAT(0.008856) ) var_X = std::pow(var_X, ( FLOAT(1)/FLOAT(3) ));
    else                           var_X = ( FLOAT(7.787) * var_X ) + ( FLOAT(16) / FLOAT(116) );

    if ( var_Y > FLOAT(0.008856) ) var_Y = std::pow(var_Y, ( FLOAT(1)/FLOAT(3) ));
    else                           var_Y = ( FLOAT(7.787) * var_Y ) + ( FLOAT(16) / FLOAT(116) );

    if ( var_Z > FLOAT(0.008856) ) var_Z = std::pow(var_Z, ( FLOAT(1)/FLOAT(3) ));
    else                           var_Z = ( FLOAT(7.787) * var_Z ) + ( FLOAT(16) / FLOAT(116) );

    return Vector3F(
        ( FLOAT(116) * var_Y ) - FLOAT(16),
        FLOAT(500) * ( var_X - var_Y ),
        FLOAT(200) * ( var_Y - var_Z ));
}

inline Vector3F lab2xyz(Vector3F const & in)
{
    FLOAT var_Y = ( in[0] + FLOAT(16) ) / FLOAT(116);
    FLOAT var_X = in[1] / FLOAT(500) + var_Y;
    FLOAT var_Z = var_Y - in[2] / FLOAT(200);

    FLOAT var_Y_3 = std::pow(var_Y,FLOAT(3));
    FLOAT var_X_3 = std::pow(var_X,FLOAT(3));
    FLOAT var_Z_3 = std::pow(var_Z,FLOAT(3));

    if ( var_X_3  > FLOAT(0.008856) ) var_X = var_X_3;
    else                              var_X = ( var_X - FLOAT(16) / FLOAT(116) ) / FLOAT(7.787);

    if ( var_Y_3  > FLOAT(0.008856) ) var_Y = var_Y_3;
    else                              var_Y = ( var_Y - FLOAT(16) / FLOAT(116) ) / FLOAT(7.787);

    if ( var_Z_3  > FLOAT(0.008856) ) var_Z = var_Z_3;
    else                              var_Z = ( var_Z - FLOAT(16) / FLOAT(116) ) / FLOAT(7.787);

    return Vector3F(
        var_X * XYZReferenceX,
        var_Y * XYZReferenceY,
        var_Z * XYZReferenceZ);
}

inline Vector3F leg2lab(Vector3F const & in)
{
    Vector3F out;
    out[0] = in[0] / FLOAT(255) * FLOAT(100);
    out[1] = in[1] - FLOAT(128) - FLOAT(1);
    out[2] = in[2] - FLOAT(128) - FLOAT(1);
    return out;
}

inline Vector3F lab2leg(Vector3F const & in)
{
    Vector3F out;
    out[0] = in[0] / FLOAT(100) * FLOAT(255);
    out[1] = in[1] + FLOAT(128) + FLOAT(1);
    out[2] = in[2] + FLOAT(128) + FLOAT(1);
    return out;
}

inline Vector3F rgb2lab(Vector3F const & in)
{
    return xyz2lab(rgb2xyz(in));
}

inline Vector3F lab2rgb(Vector3F const & in)
{
    return xyz2rgb(lab2xyz(in));
}

inline Vector3F rgb2leg(Vector3F const & in)
{
    return lab2leg(xyz2lab(rgb2xyz(in)));
}

inline Vector3F leg2rgb(Vector3F const & in)
{
    return xyz2rgb(lab2xyz(leg2lab(in)));
}

#define CLIP255(x) ((x)<0?0:(x)>255?255:(x))

inline Vector3F clipLEG(Vector3F const & in)
{
    Vector3F out;
    out[0] = in[0]<FLOAT(0)?0:in[0]>FLOAT(255)?FLOAT(255):in[0];

    FLOAT const absIn1 = std::abs(in[1]);
    FLOAT const absIn2 = std::abs(in[2]);

    if (absIn1 > FLOAT(255) || absIn2 > FLOAT(255))
    {
        FLOAT const v = std::max(absIn1, absIn2);
        out[1] = in[1] * FLOAT(255) / v;
        out[2] = in[2] * FLOAT(255) / v;
    }
    else
    {
        out[1] = in[1];
        out[2] = in[2];
    }

    return out;
}

inline Vector3F clipLAB(Vector3F const & in)
{
    Vector3F out;
    out[0] = in[0]<FLOAT(0)?0:in[0]>FLOAT(100)?FLOAT(100):in[0];

    FLOAT const absIn1 = std::abs(in[1]);
    FLOAT const absIn2 = std::abs(in[2]);

    if (absIn1 > FLOAT(128) || absIn2 > FLOAT(128))
    {
        FLOAT const v = std::max(absIn1, absIn2);
        out[1] = in[1] * FLOAT(128) / v;
        out[2] = in[2] * FLOAT(128) / v;
    }
    else
    {
        out[1] = in[1];
        out[2] = in[2];
    }

    return out;
}

inline Vector3F clipRGB(Vector3F const & in)
{
    Vector3F out;

//    auto const low  = std::min(in[0], std::min(in[1], in[2]));
//    auto const high = std::max(in[0], std::max(in[1], in[2]));

//    auto const low2  = std::max(FLOAT(0),low);
//    auto const high2 = std::min(FLOAT(255), high);

//    auto const f = (high2-low2) / (high-low);


//    if (high == low)
//    {
//        out[0] = CLIP255(in[0]);
//        out[1] = CLIP255(in[1]);
//        out[2] = CLIP255(in[2]);
//    }
//    else
//    {
//        out[0] = (in[0]-low) * f + low2;
//        out[1] = (in[1]-low) * f + low2;
//        out[2] = (in[2]-low) * f + low2;
//    }

    out[0] = CLIP255(in[0]);
    out[1] = CLIP255(in[1]);
    out[2] = CLIP255(in[2]);

    return out;
}

inline int encodeHDChannel(int const hdValue, int const original)
{
    int result;

    if (hdValue>original)
        result = (hdValue-original)*127/(255-original)+1;

    else if (original==0)
        result = 0;

    else
        result = -(hdValue*128/original);

    if (result < -128 || result > 128)
        qWarning() << "Encode error";

    return result;
}

inline int decodeHDChannel(int const encodedValue, int const original)
{
    int result;

    if (encodedValue<=0)
        result = (-encodedValue)*(original)/128;
    else
        result = original+(encodedValue-1)*(255-original)/127;

    if (result < 0 || result > 255)
        qWarning() << "Decode error";

    return result;
}

inline void encodeHDColor(uchar * const data, QRgb const hdImgColor, QRgb const refColor)
{
    int refH, refS, refV, refA;
    int hdH, hdS, hdV, hdA;

    QColor::fromRgba(refColor).getHsv(&refH, &refS, &refV, &refA);
    QColor::fromRgba(hdImgColor).getHsv(&hdH, &hdS, &hdV, &hdA);

    int offH;

    if (hdH < refH)
        offH = 360 + hdH - refH;
    else
        offH = hdH - refH;

    data[0] = (char) 255;
    data[1] = offH * 255 / 360;
    data[2] = encodeHDChannel(hdS, refS);
    data[3] = encodeHDChannel(hdV, refV);
}

inline QRgb decodeHDColor(uchar * const data, QRgb const refColor)
{
    int refH, refS, refV;
    QColor::fromRgba(refColor).getHsv(&refH, &refS, &refV);

    int const offH = data[1];
    int const offS = (char)data[2];
    int const offL = (char)data[3];

//    if (refS == 255 && data[2] == 15)
//        qWarning("Found it 1!");

    int hdH = refH + offH * 360 / 255;
    int hdS = decodeHDChannel(offS, refS);
    int hdV = decodeHDChannel(offL, refV);

    if (hdH >= 360) hdH -= 360;

    if (hdH >= 360 || hdH < -1 || hdS < 0 || hdS > 255 || hdV < 0 || hdV > 255)
        qWarning() << "Found it 2!";

    return QColor::fromHsv(hdH, hdS, hdV).rgb();
}

#define BLEND_COLORS(c1,c2,f) qRgba(\
    qRed(c1)*(1-f)+qRed(c2)*f,\
    qGreen(c1)*(1-f)+qGreen(c2)*f,\
    qBlue(c1)*(1-f)+qBlue(c2)*f,\
    qAlpha(c1)*(1-f)+qAlpha(c2)*f)

#endif // COLORS_H
