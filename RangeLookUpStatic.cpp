#include "RangeLookUpStatic.h"
RangeLookUpStatic::RangeLookUpStatic(int numOfCells, float min, float max){
    this->numOfCells = numOfCells;
    this->min = min;
    this->max = max;
    this->ranges = new Range**[numOfCells];
    this->nullRange = new Range();
    this->nullRange->roll = RangeRoll::Null;
    this->stepSize = (max - min)/numOfCells;
    for(int i = 0; i < numOfCells; i++){
        this->ranges[i] = static_cast<Range**>(malloc(sizeof(Range*)* 2));
        this->ranges[i][0] = nullRange;
        this->ranges[i][1] = nullRange;
    }
}

RangeLookUpStatic::~RangeLookUpStatic(){
    for(int i = 0; i < numOfCells; i++){
        free(this->ranges[i]);
    }
    delete this->ranges;
}

bool RangeLookUpStatic::AddRange(Range* range){
    int start = calculateIndex(range->start);
    int end = calculateIndex(range->end);
    this->ranges[start][1] = range;
    this->ranges[end][0] = range;
    for(int i = start + 1; i < end; i++){
        this->ranges[i][0] = range;
        this->ranges[i][1] = range;
    }
}

bool RangeLookUpStatic::Contains(float value){
    return this->operator[](value).roll != RangeRoll::Null;
}

Range& RangeLookUpStatic::operator[](float value){
    int index = calculateIndex(value);
    Range* resultA = ranges[index][0];
    Range* resultB = ranges[index][1];
    if(resultA->end < value){
        return *resultA;
    }else{
        return *resultB;
    }
}