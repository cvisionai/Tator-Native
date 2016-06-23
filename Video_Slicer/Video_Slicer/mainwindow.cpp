#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_chooseVideo_clicked() {

    filename = QFileDialog::getOpenFileName(this,
        tr("Choose Video"), ".",
        tr("Video Files (*.avi *.mpg *.mp4)"));
    if (!filename.isEmpty())
    {
        inputVideo.reset(new VideoCapture(filename.toLatin1().data())); // Open input
        if (!inputVideo.isOpened())
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
    }
}

void MainWindow::on_run_clicked() {

    auto pAt = filename.lastIndexOf('.');                  // Find extension point
    int ex = static_cast<int>(inputVideo->get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form
    int numFrames = (int) inputVideo->get(CV_CAP_PROP_FRAME_COUNT);
    inputVideo->set(CV_CAP_PROP_POS_FRAMES,0);
    int fps = (int) inputVideo->get(CV_CAP_PROP_FPS);

    VideoWriter outputVideo;                                        // Open the output


    const string NAME = filename.left(pAt-1).c_str()+std::to_string(n)+".avi";   // Form the new name
    outputVideo.open(NAME, ex, inputVideo->get(CV_CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened()) {
        QMessageBox msgBox;
        msgBox.setText("Could not open the output video for write!");
        msgBox.exec();
    }

    cv::Mat src;
    inputVideo->set(CV_CAP_PROP_POS_FRAMES,0);
    for(;;) //Show the image captured in the window and repeat
    {
    
        inputVideo >> src;              // read
    
        if (src.empty() | start_frame_no > end_frame_no) break;         // check if at end
        //outputVideo.write(src); //save or
        outputVideo << src;
        start_frame_no++;
    }
    
    cout << "Finished writing" << endl;
    return 0;
}
