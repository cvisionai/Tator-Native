#include "fish_detector/video_annotator/fish.h"

namespace fish_detector { namespace video_annotator {

Fish::Fish(FishTypeEnum ftype, int frame, int id)
{
	fishType = ftype;
	frameCounted = frame;
    this->id = id;
}

void Fish::setFishType(FishTypeEnum ftype)
{
	fishType = ftype;
}

void Fish::setFishSubType(int sType)
{
	subType = sType;
}

void Fish::setXYLoc(float row, float col)
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

}} // namespace fish_detector::video_annotator

