#include "csvio.h"

void readFromCSV()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Video"), ".",
        tr("Video Files (*.avi *.mpg *.mp4)"));

    //QFileInfo name = filename;
    ifstream inFile(filename,ios::in);
    string line;
    stringstream linestream;
    string tripID, reviewer, towType, fishType, species;
    int frame, towNum;
    double timeInVid;
    getline(inFile,line,",");
    line.clear();
    while(getline(inFile,line))
    {
        linestream << line;
        line.clear();

        //Assign to the members of the struct
        tripID << linestream;
        towNum << linestream;
        reviewer << linestream;
        towType << linestream;
        fishType << linestream;
        species << linestream;
        frame << linestream;
        timeInVid << linestream;
        FishTypeEnum fType = getFishType(fishType);
        unique_ptr<Fish> tempFish(new Fish(fType,currentFrame));
        tempFish->setFishSubType(getFishSpecies(fType,species));
        myFishList.push_back(*tempFish);

        linestream.str("");
        linestream.clear();
    }
    inFile.close();
    ui->totalFishVal->setText(QString::number(myFishList.size()));
    listPos = myFishList.begin();
    ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
    ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
    updateVecIndex();
}

FishTypeEnum getFishType (string const& inString)
{
    if (inString == "ROUND") return ROUND;
    if (inString == "FLAT") return FLAT;
    if (inString == "SKATE") return SKATE;
    if (inString == "OTHER") return OTHER;
}

int getFishSpecies (FishTypeEnum fType, string const& sString)
{
    switch (fType)
    case ROUND:
        if (sString == "Round") return 0;
        if (sString == "Haddock") return 1;
        if (sString == "Cod") return 2;
        if (sString == "Whiting") return 3;
        if (sString == "Red Hake") return 4;
        if (sString == "Pollock") return 5;
        if (sString == "Herring") return 6;
        if (sString == "Unknown") return 7;
    case FLAT:
        if (sString == "Flat") return 0;
        if (sString == "Yellowtail") return 1;
        if (sString == "Windowpane") return 2;
        if (sString == "Summer") return 3;
        if (sString == "FourSport") return 4;
        if (sString == "Grey Sole") return 5;
        if (sString == "Halibut") return 6;
        if (sString == "Unknown") return 7;
    case SKATE:
        if (sString == "Skate") return 0;
        if (sString == "Barndoor") return 1;
        if (sString == "Unknown") return 2;
    case OTHER:
        if (sString == "Other") return 0;
        if (sString == "Dogfish") return 1;
        if (sString == "Monkfish") return 2;
        if (sString == "Lobster") return 3;
        if (sString == "Scallop") return 4;
}
