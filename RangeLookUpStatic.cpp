#ifndef _RANGE_LOOKUP_STATIC_CPP_
#define _RANGE_LOOKUP_STATIC_CPP_
#include "RangeLookUpStatic.h"

template<typename T>
RangeLookUpStatic<T>::RangeLookUpStatic(int numOfCells, float min, float max){
    this->numOfCells = numOfCells;
    this->min = min;
    this->max = max;
    this->ranges = new Range<T>**[numOfCells];
    this->nullRange = new Range<T>(0, 0, RangeRoll::Null);
    this->stepSize = (max - min)/numOfCells;
    this->capacities = static_cast<int*>(malloc(sizeof(int) * numOfCells));
    this->numOfMembers = static_cast<int*>(malloc(sizeof(int) * numOfCells));
    for(int i = 0; i < numOfCells; i++){
        this->ranges[i] = static_cast<Range<T>**>(malloc(sizeof(Range<T>*) * 2));
        this->ranges[i][0] = nullRange;
        this->ranges[i][1] = nullRange;
        this->capacities[i] = 2;
        this->numOfMembers[i] = 0;
    }
}

template<typename T>
RangeLookUpStatic<T>::~RangeLookUpStatic(){
    for(int i = 0; i < numOfCells; i++){
        free(this->ranges[i]);
    }
    delete this->ranges;
    delete this->nullRange;
    for(const Range<T>* range : this->rangePile){
        delete range;
    }
    free(this->capacities);
    free(this->numOfMembers);
}

template<typename T>
bool RangeLookUpStatic<T>::AddRange(float xA, float xB, T payload){
    Range<T>* range = new Range<T>(xA, xB, RangeRoll::Normal, payload);
    rangePile.push_back(range);
    int start = calculateIndex(range->start);
    int end = calculateIndex(range->end);
    for(int i = start; i <= end; i++){
        StoreRange(range, i);
    }
    return true; // TODO implement validations for overlap and success of operation
}

template<typename T>
bool RangeLookUpStatic<T>::Contains(float value){
    return this->operator[](value).roll != RangeRoll::Null;
}

template<typename T>
Range<T>& RangeLookUpStatic<T>::operator[](float value){
    int index = calculateIndex(value);
    for(int i = 0; i < this->numOfMembers[index]; i++){
        if(this->ranges[index][i]->start <= value && this->ranges[index][i]->end >= value){
            return *this->ranges[index][i];
        }else if(this->ranges[index][i]->start > value){
            return *this->nullRange;
        }
    }
    return *nullRange;
}

#endif