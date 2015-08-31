#include "fish.h"

Fish::Fish(FishTypeEnum ftype, int frame)
{
	fishType = ftype;
	frameCounted = frame;
}

Fish::setFishType(FishTypeEnum type)
{
	fishType = ftype;
}

Fish::setFishSubType(int sType)
{
	subType = sType;
}

Fish::setXYLoc(int row, int col)
{
	fishLoc = std::make_pair(row,col);
}

FishTypeEnum Fish::getFishType()
{
	return fishType;
}

int Fish::getFishSubType()
{
	return subType;
}

int Fish::getFrameCounted()
{
	return frameCounted;
}

xyLoc Fish::getXYLoc()
{
	return fishLoc;
}
