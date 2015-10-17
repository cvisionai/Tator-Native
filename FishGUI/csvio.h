#ifndef CSVIO
#define CSVIO
#include<iostream>
#include<fstream>
#include<sstream>

#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>

#include "fish.h"


void readFromCSV();
FishTypeEnum getFishType(string const& fString, int frame);
int getFishSpecies(FishTypeEnum fType, string const& sString);

#endif // CSVIO

