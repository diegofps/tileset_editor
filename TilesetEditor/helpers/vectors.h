#ifndef HELPERS__VECTORS_H
#define HELPERS__VECTORS_H

#include <QApplication>
#include <initializer_list>
#include <iostream>

typedef double FLOAT;

template <typename T>
class Vector2
{
public:

    Vector2()
    {

    }

    Vector2(T const x, T const y)
    {
        _data[0] = x;
        _data[1] = y;
    }

    Vector2(std::initializer_list<T> l)
    {
        *this = l;
    }

    T & operator[](int const i)
    {
        return _data[i];
    }

    T const & operator[](int const i) const
    {
        return _data[i];
    }

    T & x()
    {
        return _data[0];
    }

    T & y()
    {
        return _data[1];
    }

    T x() const
    {
        return _data[0];
    }

    T y() const
    {
        return _data[1];
    }

    void operator=(std::initializer_list<T> l)
    {
        if (l.size() != 2)
        {
            qWarning() << "Invalid initializer size";
            return;
        }

        _data[0] = l.begin()[0];
        _data[1] = l.begin()[1];
    }

    bool operator==(Vector2<T> const & other)
    {
        for (int i = 0; i != 2; ++i)
            if (_data[i] != other._data[i])
                return false;
        return true;
    }

    Vector2<T> & operator+=(Vector2<T> const & other)
    {
        _data[0] += other._data[0];
        _data[1] += other._data[1];
        return *this;
    }

    Vector2<T> & operator-=(Vector2<T> const & other)
    {
        _data[0] -= other._data[0];
        _data[1] -= other._data[1];
        return *this;
    }

    Vector2<T> & operator*=(T const value)
    {
        _data[0] *= value;
        _data[1] *= value;
        return *this;
    }

    Vector2<T> & operator/=(T const value)
    {
        _data[0] /= value;
        _data[1] /= value;
        return *this;
    }

    Vector2<T> operator+(T const value) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] + value;
        tmp._data[1] = _data[1] + value;
        return tmp;
    }

    Vector2<T> operator-(T const value) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] - value;
        tmp._data[1] = _data[1] - value;
        return tmp;
    }

    Vector2<T> operator*(T const value) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] * value;
        tmp._data[1] = _data[1] * value;
        return tmp;
    }

    Vector2<T> operator/(T const value) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] / value;
        tmp._data[1] = _data[1] / value;
        return tmp;
    }

    Vector2<T> operator+(Vector2<T> const & other) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] + other._data[0];
        tmp._data[1] = _data[1] + other._data[1];
        return tmp;
    }

    Vector2<T> operator-(Vector2<T> const & other) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] - other._data[0];
        tmp._data[1] = _data[1] - other._data[1];
        return tmp;
    }

    Vector2<T> operator*(Vector2<T> const & value) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] * value[0];
        tmp._data[1] = _data[1] * value[1];
        return tmp;
    }

    Vector2<T> operator/(Vector2<T> const & value) const
    {
        Vector2<T> tmp;
        tmp._data[0] = _data[0] / value[0];
        tmp._data[1] = _data[1] / value[1];
        return tmp;
    }

    T module2() const
    {
        T res = 0;
        res += _data[0] * _data[0];
        res += _data[1] * _data[1];
        return res;
    }

    T module() const
    {
        return std::sqrt(module2());
    }

    Vector2<T> normalize() const
    {
        Vector2<T> out;
        T const d = module();
        out[0] = _data[0] / d;
        out[1] = _data[1] / d;
        return out;
    }

    Vector2<T> flip() const
    {
        return Vector2<T>(_data[1], _data[0]);
    }

private:

    T _data[2];

};

template <typename T>
std::ostream & operator<<(std::ostream & o, Vector2<T> const & c)
{
    o << "(" << c[0] << "," << c[1] << ")";
    return o;
}

template <typename T>
class Vector3
{
public:

    Vector3()
    {

    }

    Vector3(T const x, T const y, T const z)
    {
        _data[0] = x;
        _data[1] = y;
        _data[2] = z;
    }

    Vector3(std::initializer_list<T> l)
    {
        *this = l;
    }

    T & operator[](int const i)
    {
        return _data[i];
    }

    T const & operator[](int const i) const
    {
        return _data[i];
    }

    T & x()
    {
        return _data[0];
    }

    T & y()
    {
        return _data[1];
    }

    T & z()
    {
        return _data[2];
    }

    T x() const
    {
        return _data[0];
    }

    T y() const
    {
        return _data[1];
    }

    T z() const
    {
        return _data[2];
    }

    void operator=(std::initializer_list<T> l)
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

    bool operator==(Vector3<T> const & other)
    {
        for (int i = 0; i != 3; ++i)
            if (_data[i] != other._data[i])
                return false;
        return true;
    }

    Vector3<T> & operator+=(Vector3<T> const & other)
    {
        _data[0] += other._data[0];
        _data[1] += other._data[1];
        _data[2] += other._data[2];
        return *this;
    }

    Vector3<T> & operator-=(Vector3<T> const & other)
    {
        _data[0] -= other._data[0];
        _data[1] -= other._data[1];
        _data[2] -= other._data[2];
        return *this;
    }

    Vector3<T> & operator*=(T const value)
    {
        _data[0] *= value;
        _data[1] *= value;
        _data[2] *= value;
        return *this;
    }

    Vector3<T> & operator/=(T const value)
    {
        _data[0] /= value;
        _data[1] /= value;
        _data[2] /= value;
        return *this;
    }

    Vector3<T> operator+(T const value) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] + value;
        tmp._data[1] = _data[1] + value;
        tmp._data[2] = _data[2] + value;
        return tmp;
    }

    Vector3<T> operator-(T const value) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] - value;
        tmp._data[1] = _data[1] - value;
        tmp._data[2] = _data[2] - value;
        return tmp;
    }

    Vector3<T> operator*(T const value) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] * value;
        tmp._data[1] = _data[1] * value;
        tmp._data[2] = _data[2] * value;
        return tmp;
    }

    Vector3<T> operator/(T const value) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] / value;
        tmp._data[1] = _data[1] / value;
        tmp._data[2] = _data[2] / value;
        return tmp;
    }

    Vector3<T> operator+(Vector3<T> const & other) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] + other._data[0];
        tmp._data[1] = _data[1] + other._data[1];
        tmp._data[2] = _data[2] + other._data[2];
        return tmp;
    }

    Vector3<T> operator-(Vector3<T> const & other) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] - other._data[0];
        tmp._data[1] = _data[1] - other._data[1];
        tmp._data[2] = _data[2] - other._data[2];
        return tmp;
    }

    Vector3<T> operator*(Vector3<T> const & value) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] * value[0];
        tmp._data[1] = _data[1] * value[1];
        tmp._data[2] = _data[2] * value[2];
        return tmp;
    }

    Vector3<T> operator/(Vector3<T> const & value) const
    {
        Vector3<T> tmp;
        tmp._data[0] = _data[0] / value[0];
        tmp._data[1] = _data[1] / value[1];
        tmp._data[2] = _data[2] / value[2];
        return tmp;
    }

    Vector3<T> sign() const
    {
        Vector3<T> res;
        res[0] = _data[0] >= -1 ? +1 : -1;
        res[1] = _data[1] >= -1 ? +1 : -1;
        res[2] = _data[2] >= -1 ? +1 : -1;
        return res;
    }

    T module2() const
    {
        T res = 0;
        res += _data[0] * _data[0];
        res += _data[1] * _data[1];
        res += _data[2] * _data[2];
        return res;
    }

    T module() const
    {
        return std::sqrt(module2());
    }

    Vector3<T> normalize() const
    {
        Vector3<T> out;
        T const d = module();
        out[0] = _data[0] / d;
        out[1] = _data[1] / d;
        out[2] = _data[2] / d;
        return out;
    }

    Vector3<T> flip() const
    {
        return Vector3<T>(_data[2], _data[1], _data[0]);
    }

    Vector2<T> sub0() const
    {
        return Vector2<T>(_data[0], _data[1]);
    }

    Vector2<T> sub1() const
    {
        return Vector2<T>(_data[1], _data[2]);
    }

    Vector2<T> sub2() const
    {
        return Vector2<T>(_data[2], _data[0]);
    }

private:

    T _data[3];

};

template <typename T>
std::ostream & operator<<(std::ostream & o, Vector3<T> const & c)
{
    o << "(" << c[0] << "," << c[1] << "," << c[2] << ")";
    return o;
}

template <typename T>
class Matrix2
{
public:

    Matrix2()
    {

    }

    Matrix2(T const v00, T const v01, T const v10, T const v11)
    {
        _data[0] = v00;
        _data[1] = v01;
        _data[2] = v10;
        _data[3] = v11;
    }

    Vector2<T> operator*(Vector2<T> const & v) const
    {
        Vector2<T> out;
        out[0] = _data[0] * v[0] + _data[1] * v[1];
        out[1] = _data[2] * v[0] + _data[3] * v[1];
        return out;
    }

    Matrix2<T> operator*(Matrix2<T> const & in) const
    {
        Matrix2<T> out;
        out(0,0) = (*this)(0,0) * in(0,0) + (*this)(0,1) * in(1,0);
        out(0,1) = (*this)(0,0) * in(0,1) + (*this)(0,1) * in(1,1);
        out(1,0) = (*this)(1,0) * in(0,0) + (*this)(1,1) * in(1,0);
        out(1,1) = (*this)(1,0) * in(0,1) + (*this)(1,1) * in(1,1);
        return out;
    }

    T & operator()(int const i, int const j)
    {
        return _data[i*2+j];
    }

    T operator()(int const i, int const j) const
    {
        return _data[i*2+j];
    }

private:

    T _data[2*2];
};

template <typename T>
std::ostream & operator<<(std::ostream & o, Matrix2<T> const & m)
{
    o << "((" << m(0,0) << "," << m(0,1) << "),(" << m(1,0) << "," << m(1,1) <<  "))";
    return o;
}

template <typename T>
Matrix2<T> rotationMatrix(Vector2<T> const & m1, Vector2<T> const & m2)
{
    Matrix2<T> out;
    out(0,0) = m1.x() * m2.x() + m1.y() * m2.y();
    out(0,1) = m2.x() * m1.y() - m1.x() * m2.y();
    out(1,0) = m1.x() * m2.y() - m2.x() * m1.y();
    out(1,1) = m1.x() * m2.x() + m1.y() * m2.y();

    return out;
}

template <typename T>
Matrix2<T> scaleMatrix(T const v)
{
    Matrix2<T> out;
    out(0,0) = v;
    out(0,1) = 0;
    out(1,0) = 0;
    out(1,1) = v;
    return out;
}

typedef Vector2<int> Vector2I;
typedef Vector2<FLOAT> Vector2F;

typedef Vector3<int> Vector3I;
typedef Vector3<FLOAT> Vector3F;

typedef Matrix2<int> Matrix2I;
typedef Matrix2<FLOAT> Matrix2F;

#endif // HELPERS__VECTORS_H

