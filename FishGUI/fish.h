#include <utility>

typedef std::pair<float, float> xyLoc;
enum FishTypeEnum
{
  Round,
  Flat,
  Skate
};

class Fish
{
private:
	FishTypeEnum fishType;
	int subType;
	int frameCounted;
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
