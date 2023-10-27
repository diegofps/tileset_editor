#ifndef RANGE_H
#define RANGE_H

#include <algorithm>

class Range
{
public:

    Range() :
        start(-1),
        end(-1)
    {

    }

    Range(int start, int end) :
        start(start),
        end(end)
    {

    }

    bool isEmpty() const
    {
        return end < start;
    }

    void limit(int const min, int const max)
    {
        start = std::max(min, start);
        start = std::min(max, start);

        end = std::max(min, end);
        end = std::min(max, end);
    }

    void set(int const start, int const end)
    {
        this->start = start;
        this->end = end;
    }

    bool operator!=(Range const & other) const
    {
        return !(*this == other);
    }

    bool operator==(Range const & other) const
    {
        return start == other.start && end == other.end;
    }

public:

    int start;
    int end;

};

#endif // RANGE_H
