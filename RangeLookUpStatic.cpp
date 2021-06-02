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
    for(int i = 0; i < numOfCells; i++){
        this->ranges[i] = static_cast<Range<T>**>(malloc(sizeof(Range<T>*)* 2));
        this->ranges[i][0] = nullRange;
        this->ranges[i][1] = nullRange;
    }
}

template<typename T>
RangeLookUpStatic<T>::~RangeLookUpStatic(){
    for(int i = 0; i < numOfCells; i++){
        free(this->ranges[i]);
    }
    delete this->ranges;
}

template<typename T>
bool RangeLookUpStatic<T>::AddRange(float xA, float xB, T payload){
    Range<T> range = Range<T>(xA, xB, RangeRoll::Normal, payload);
    rangePile.push_back(range);
    int start = calculateIndex(range.start);
    int end = calculateIndex(range.end);
    Range<T>* rangePointer = &rangePile.back();
    this->ranges[start][1] = rangePointer;
    this->ranges[end][0] = rangePointer;
    for(int i = start + 1; i < end; i++){
        this->ranges[i][0] = rangePointer;
        this->ranges[i][1] = rangePointer;
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
    Range<T>* resultA = ranges[index][0];
    Range<T>* resultB = ranges[index][1];
    //careful: end >= value and start <= value has an overlap at end == start
    if(resultA->roll != RangeRoll::Null && resultA->end >= value){
        return *resultA;
    }else if(resultB->start <= value){
        // this check is needed when result A is a Null range
        return *resultB;
    }
    return *nullRange;
}

#endif