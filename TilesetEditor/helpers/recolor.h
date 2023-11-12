#ifndef RECOLOR_H
#define RECOLOR_H

#include <QImage>

#include "helpers/vectors.h"
#include "helpers/colors.h"

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

    int getAlpha(size_t const i, size_t const j) const
    {
        return qRed(_img.pixel(j,i));
    }

    void setPixel(size_t i, size_t j, Vector3F const & color, int const alpha) const
    {
        auto const values = qRgba(CLIP255(color[0]), CLIP255(color[1]), CLIP255(color[2]), alpha);
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
inline void recolor12(std::vector<Vector3F> const & colors1,
                      std::vector<Vector3F> const & colors2,
                      IMAGE_WRAPPER const & imgIn,
                      IMAGE_WRAPPER const & maskIn,
                      IMAGE_WRAPPER & imgOut)
{
    size_t const k = std::min(colors1.size(), colors2.size());

    std::vector<Vector3F> labs1(k);
    std::vector<Vector3F> labs2(k);
    std::vector<Vector3F> deltas(k);

    for (size_t i=0;i!=k;++i)
    {
        auto const v1 = rgb2leg(colors1[i]);
        auto const v2 = rgb2leg(colors2[i]);

        labs1 [i] = v1;
        labs2 [i] = v2;
        deltas[i] = v2/v1;
    }

    for (size_t i = 0; i != imgIn.rows(); ++i)
    {
        for (size_t j = 0; j != imgIn.cols(); ++j)
        {
            if (i == 30 && j == 77)
                qWarning() << "found it 2";

            Vector3F rgbIn = imgIn.getPixel(i, j);
            Vector3F labIn = rgb2leg(rgbIn);

            FLOAT  bestDistance1;
            FLOAT  bestDistance2;
            FLOAT  bestDistance3;

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

            Vector3F delta {0,0,0};
            FLOAT w1, w2, w3;

            if (bestP1 != k && bestP2 != k && bestP3 != k)
            {
                w1 = 1/(bestDistance1+1);
                w2 = 1/(bestDistance2+1);
                w3 = 1/(bestDistance3+1);

                delta = (deltas[bestP1] * w1 +
                         deltas[bestP2] * w2 +
                         deltas[bestP3] * w3) / (w1 + w2 + w3);
            }
            else if (bestP1 != k && bestP2 != k)
            {
                w1 = 1/(bestDistance1+1);
                w2 = 1/(bestDistance2+1);

                delta = (deltas[bestP1] * w1 +
                         deltas[bestP2] * w2) / (w1 + w2);
            }

            Vector3F rgbOut = leg2rgb(labIn * delta);
            imgOut.setPixel(i, j, clipRGB(rgbOut), maskIn.getAlpha(i,j));
        }
    }
}

#endif // RECOLOR_H
