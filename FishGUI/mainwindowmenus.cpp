#include <mainwindow.h>


void MainWindow::updateTypeMenu()
{
    QStringList typeList;
    typeList.append("Round");
    typeList.append("Flat");
    typeList.append("Skate");
    typeList.append("Other");
    ui->typeMenu->addItems(typeList);
}


void MainWindow::updateSubTypeMenu(int typeIndex)
{
    QStringList sTypeList;
    ui->subTypeMenu->blockSignals(true);
    ui->subTypeMenu->clear();
    if (typeIndex == 0) {
        sTypeList.append("Round");
        sTypeList.append("Haddock");
        sTypeList.append("Cod");
        sTypeList.append("Whiting");
        sTypeList.append("Red Hake");
        sTypeList.append("Pollock");
        sTypeList.append("Herring");
        sTypeList.append("Unknown");
    }
    else if (typeIndex == 1) {
        sTypeList.append("Flat");
        sTypeList.append("Yellowtail");
        sTypeList.append("Windowpane");
        sTypeList.append("Summer");
        sTypeList.append("Winter");
        sTypeList.append("Fourspot");
        sTypeList.append("Grey Sole");
        sTypeList.append("Halibut");
        sTypeList.append("Unknown");
    }
    else if (typeIndex == 2) {
        sTypeList.append("Skate");
        sTypeList.append("Barndoor");
        sTypeList.append("Unknown");
    }
    else if (typeIndex == 3) {
        sTypeList.append("Other");
        sTypeList.append("Dogfish");
        sTypeList.append("Monkfish");
        sTypeList.append("Lobster");
        sTypeList.append("Scallop");
    }
    ui->subTypeMenu->addItems(sTypeList);
    ui->subTypeMenu->setCurrentIndex(0);
    ui->subTypeMenu->blockSignals(false);
}

FishTypeEnum MainWindow::getFishType (string const& inString)
{
    if (inString == "ROUND") return ROUND;
    if (inString == "FLAT") return FLAT;
    if (inString == "SKATE") return SKATE;
    if (inString == "OTHER") return OTHER;
    return OTHER;
}

int MainWindow::getFishSpecies (FishTypeEnum fType, string const& sString)
{
    switch (fType)
    {
    case ROUND:
        if (sString == "Round") return 0;
        if (sString == "Haddock") return 1;
        if (sString == "Cod") return 2;
        if (sString == "Whiting") return 3;
        if (sString == "Red Hake") return 4;
        if (sString == "Pollock") return 5;
        if (sString == "Herring") return 6;
        if (sString == "Unknown") return 7;
        return 0;
        break;
    case FLAT:
        if (sString == "Flat") return 0;
        if (sString == "Yellowtail") return 1;
        if (sString == "Windowpane") return 2;
        if (sString == "Summer") return 3;
        if (sString == "FourSport") return 4;
        if (sString == "Grey Sole") return 5;
        if (sString == "Halibut") return 6;
        if (sString == "Unknown") return 7;
        return 0;
        break;
    case SKATE:
        if (sString == "Skate") return 0;
        if (sString == "Barndoor") return 1;
        if (sString == "Unknown") return 2;
        return 0;
        break;
    case OTHER:
        if (sString == "Other") return 0;
        if (sString == "Dogfish") return 1;
        if (sString == "Monkfish") return 2;
        if (sString == "Lobster") return 3;
        if (sString == "Scallop") return 4;
        return 0;
        break;
    default:
        return 0;
        break;
    }
}

string MainWindow::getFishTypeString (FishTypeEnum fType)
{
    switch (fType){
    case ROUND:
        return "ROUND";
        break;
    case FLAT:
        return "FLAT";
        break;
    case SKATE:
        return "SKATE";
        break;
    case OTHER:
        return "OTHER";
        break;
    default:
        return "OTHER";
        break;
    }
}

string MainWindow::getFishSpeciesString (FishTypeEnum fType, int species)
{
    switch (fType)
    {
    case ROUND:
        if (species == 0) return "Round";
        if (species == 1) return "Haddock";
        if (species == 2) return "Cod";
        if (species == 3) return "Whiting";
        if (species == 4) return "Red Hake";
        if (species == 5) return "Pollock";
        if (species == 6) return "Herring";
        if (species == 7) return "Unknown";
        return "Unknown";
        break;
    case FLAT:
        if (species == 0) return "Flat";
        if (species == 1) return "Yellowtail";
        if (species == 2) return "Windowpane";
        if (species == 3) return "Summer";
        if (species == 4) return "FourSport";
        if (species == 5) return "Grey Sole";
        if (species == 6) return "Halibut";
        if (species == 7) return "Unknown";
        return "Unknown";
        break;
    case SKATE:
        if (species == 0) return "Skate";
        if (species == 1) return "Barndoor";
        if (species == 2) return "Unknown";
        return "Unknown";
        break;
    case OTHER:
        if (species == 0) return "Other";
        if (species == 1) return "Dogfish";
        if (species == 2) return "Monkfish";
        if (species == 3) return "Lobster";
        if (species == 4) return "Scallop";
        return "Unknown";
        break;
    default:
        return "Unknown";
        break;
    }
}
