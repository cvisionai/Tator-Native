typedef std::pair<int, int> xyLoc;
class Fish
{
public:
	enum FishTypeEnum
	{
	  Unknown,
	  Flounder,
	  Skate,
	  Cod,
	  Haddock
	};
private:
	FishTypeEnum fishType;
	int subType;
	int frameCounted;
	xyLoc fishLoc;

public:
	Fish(FishTypeEnum fType, int frame);
	void setFishType(FishTypeEnum type);
	void setFishSubType(int sType);
	void setXYLoc(int row, int col);
	FishTypeEnum getFishType();
	int getFishSubType();
	int getFrameCounted();
	xyLoc getXYLoc();
};
