#pragma once
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>


/// todo:
/// Range overlap is not handled
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
    std::vector<Range<T>*> rangePile;
    int* capacities;
    int* numOfMembers;
    Range<T>* nullRange;
    Range<T>*** ranges;  // [[Range* , ..., Range*], ... ]

    public:
    RangeLookUpStatic() = delete;
    RangeLookUpStatic(int numOfCells, float min, float max);
    ~RangeLookUpStatic();
    bool AddRange(float start, float end, T payload);
    bool Contains(float value);
    Range<T>& operator [](float value);

    private:
    int calculateIndex(float value){
        return std::min((int)((value - min) / stepSize), numOfCells - 1);  //min(..., numOfCells - 1) the index for max value needs to be covered
    }

    void StoreRange(Range<T>* range, int index){
        if(this->numOfMembers[index] == this->capacities[index]){
            ResizeStorage(index, this->capacities[index] + 10);
        }
        for(int i = 0; i < this->numOfMembers[index]; i++){
            if(this->ranges[index][i]->start >= range->end){
                InsertRangeIntoStorage(range, index, i);
                return;
            }
            if(this->ranges[index][i]->roll == RangeRoll::Null){
                InsertRangeIntoStorage(range, index, i);
                return;
            }
        }
        InsertRangeIntoStorage(range, index, this->numOfMembers[index]);
    }

    void InsertRangeIntoStorage(Range<T>* range, int index, int position){
        for(int i = this->numOfMembers[index] - 1; i >= position; i--){
            this->ranges[index][i + 1] = this->ranges[index][i];
        }
        this->ranges[index][position] = range;
        this->numOfMembers[index]++;
    }

    void ResizeStorage(int index, int size){
        Range<T>** cache = static_cast<Range<T>**>(malloc(sizeof(Range<T>*) * size));
        std::copy(ranges[index], ranges[index] + this->numOfMembers[index], cache);
        free(this->ranges[index]);
        for(int i = this->capacities[index]; i < size; i++){
            cache[i] = nullRange;
        }
        this->ranges[index] = cache;
        this->capacities[index] = size;
    }
};

#include "RangeLookUpStatic.cpp"