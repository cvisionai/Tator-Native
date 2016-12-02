#include <QtTest/QtTest>

class TestImageAnnotator : public QObject {
  Q_OBJECT
private slots:
  void testLoadImageDir();
  void testLoadSpeciesFile();
  void testAddRegionAndNext();
};
