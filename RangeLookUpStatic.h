#include <stdlib.h>


/// todo:
/// each Range* cell can only store 2 Pointers;
///     Range overlap is not handled
/// RangeCoverage is not guaranteed to be "dense"
enum class RangeRoll{
    Normal,
    Null
};

struct Range{
    float start;
    float end;
    RangeRoll roll;
};

class RangeLookUpStatic {
    private:
    int numOfCells;
    float min;
    float max;
    float stepSize;
    Range* nullRange;
    Range*** ranges;  // [[Range* , Range*], ... ]

    public:
    RangeLookUpStatic() = delete;
    RangeLookUpStatic(int numOfCells, float min, float max);
    ~RangeLookUpStatic();
    bool AddRange(Range*);
    bool Contains(float value);
    Range& operator [](float value);

    private:
    int calculateIndex(float value){
        return (value - min) / stepSize;
    }
};