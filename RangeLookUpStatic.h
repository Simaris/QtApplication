#pragma once
#include <stdlib.h>
#include <vector>
#include <algorithm>


/// todo:
/// each Range* cell can only store 2 Pointers;
///     Range overlap is not handled
/// RangeCoverage is not guaranteed to be "dense"
enum class RangeRoll{
    Normal,
    Null
};

template<typename T>
struct Range{
    float start;
    float end;
    RangeRoll roll;
    T payload;
    Range() = delete;
    Range(float start, float end, RangeRoll roll){
        this->start = start;
        this->end = end;
        this->roll = roll;
    }
    Range(float start, float end, RangeRoll roll, T payload){
        this->start = start;
        this->end = end;
        this->roll = roll;
        this->payload = payload;
    }
};

template<typename T>
class RangeLookUpStatic {
    private:
    int numOfCells;
    float min;
    float max;
    float stepSize;
    std::vector<Range<T>> rangePile;
    Range<T>* nullRange;
    Range<T>*** ranges;  // [[Range* , Range*], ... ]

    public:
    RangeLookUpStatic() = delete;
    RangeLookUpStatic(int numOfCells, float min, float max);
    ~RangeLookUpStatic();
    bool AddRange(float, float, T);
    bool Contains(float value);
    Range<T>& operator [](float value);

    private:
    int calculateIndex(float value){
        return std::min((int)((value - min) / stepSize), numOfCells - 1);  //min(..., numOfCells - 1) the index for max value needs to be covered
    }
};

#include "RangeLookUpStatic.cpp"