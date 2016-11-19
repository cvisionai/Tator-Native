#include <QtTest/QtTest>

class TestVideoAnnotator : public QObject {
  Q_OBJECT
private slots:
  void testLoadVideo();
  void testSaveAnnotationsNoFish();
};
