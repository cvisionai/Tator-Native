//#include <QtTest/QtTest>
#include <QObject>
#include <string>
//#include <memory>
//#include "../FishGUI/document.h"
// unit test class for Document
using namespace std;

class TestDocument : public QObject {
    Q_OBJECT
private slots:
    void writeJSON();
};
