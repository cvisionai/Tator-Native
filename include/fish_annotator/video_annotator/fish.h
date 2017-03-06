#ifndef FISH_H
#define FISH_H

#include <utility>

namespace fish_annotator { namespace video_annotator {

typedef std::pair<float, float> xyLoc;

enum FishTypeEnum
{
  ROUND,
  FLAT,
  SKATE,
  OTHER,
  DNC
};

class Fish
{
public:
    bool operator < (const Fish& rFish) const
    {
        return (frameCounted < rFish.frameCounted);
    }
public:
    int frameCounted;

private:
	FishTypeEnum fishType;
	int subType;
	xyLoc fishLoc;
    int id;

public:
    Fish(FishTypeEnum fType, int frame, int id);
	void setFishType(FishTypeEnum type);
	void setFishSubType(int sType);
	void setXYLoc(float row, float col);
	FishTypeEnum getFishType();
	int getFishSubType();
	int getFrameCounted();
	xyLoc getXYLoc();
    int getID(){return id;}
};

}} // namespace fish_annotator::video_annotator

#endif // FISH_H
