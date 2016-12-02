#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <memory>
#include <string>

#include <QWidget>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

namespace fish_annotator { namespace video_slicer {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_chooseVideo_clicked();
    void on_run_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<cv::VideoCapture> inputVideo;
    QString filename;
};

}} // namespace fish_annotator::video_slicer

#endif // MAINWINDOW_H
