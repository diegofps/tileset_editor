#ifndef HELPERS__VECTORS_H
#define HELPERS__VECTORS_H

#include <QApplication>
#include <initializer_list>
#include <iostream>

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

inline std::ostream & operator<<(std::ostream & o, Vector2F const & c)
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

    Vector3F sign() const
    {
        Vector3F res;
        res[0] = _data[0] >= -1.0f ? +1.0f : -1.0f;
        res[1] = _data[1] >= -1.0f ? +1.0f : -1.0f;
        res[2] = _data[2] >= -1.0f ? +1.0f : -1.0f;
        return res;
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

inline std::ostream & operator<<(std::ostream & o, Vector3F const & c)
{
    o << "(" << c[0] << "," << c[1] << "," << c[2] << ")";
    return o;
}

class Matrix2F
{
public:

    Matrix2F()
    {

    }

    Matrix2F(FLOAT const v00, FLOAT const v01, FLOAT const v10, FLOAT const v11)
    {
        _data[0] = v00;
        _data[1] = v01;
        _data[2] = v10;
        _data[3] = v11;
    }

    Vector2F operator*(Vector2F const & v) const
    {
        Vector2F out;
        out[0] = _data[0] * v[0] + _data[1] * v[1];
        out[1] = _data[2] * v[0] + _data[3] * v[1];
        return out;
    }

    FLOAT & operator()(int const i, int const j)
    {
        return _data[i*2+j];
    }

    FLOAT operator()(int const i, int const j) const
    {
        return _data[i*2+j];
    }

private:

    FLOAT _data[2*2];
};

inline std::ostream & operator<<(std::ostream & o, Matrix2F const & m)
{
    o << "((" << m(0,0) << "," << m(0,1) << "),(" << m(1,0) << "," << m(1,1) <<  "))";
    return o;
}

#endif // HELPERS__VECTORS_H

