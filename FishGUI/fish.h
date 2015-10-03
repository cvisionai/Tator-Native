#include <utility>

typedef std::pair<float, float> xyLoc;

enum FishTypeEnum
{
  ROUND,
  FLAT,
  SKATE,
  OTHER
};

class Fish
{
public:
    bool operator < (const Fish& str) const
    {
        return (frameCounted < str.frameCounted);
    }
public:
    int frameCounted;

private:
	FishTypeEnum fishType;
	int subType;
	xyLoc fishLoc;

public:
	Fish(FishTypeEnum fType, int frame);
	void setFishType(FishTypeEnum type);
	void setFishSubType(int sType);
	void setXYLoc(float row, float col);
	FishTypeEnum getFishType();
	int getFishSubType();
	int getFrameCounted();
	xyLoc getXYLoc();
};
