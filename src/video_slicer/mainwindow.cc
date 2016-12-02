#include <string>

#include "fish_annotator/video_slicer/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace video_slicer {

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
        inputVideo.reset(new cv::VideoCapture(filename.toLatin1().data())); // Open input
        if (!inputVideo->isOpened())
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
    }
}

void MainWindow::on_run_clicked() {

    auto pAt = filename.lastIndexOf('.'); // Find extension point
//    int ex = static_cast<int>(inputVideo->get(CV_CAP_PROP_FOURCC)); // Get Codec Type- Int form
    int numFrames = (int) inputVideo->get(CV_CAP_PROP_FRAME_COUNT); //Number of frames in video
    inputVideo->set(CV_CAP_PROP_POS_FRAMES,0);
    double fps = inputVideo->get(CV_CAP_PROP_FPS);
    int secondsPerSlice = 60;
    int framesPerSlice = (double) secondsPerSlice * fps;
    int numSlices = (int)((double) numFrames / (double) framesPerSlice);
    cv::Size S = cv::Size((int) inputVideo->get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                      (int) inputVideo->get(CV_CAP_PROP_FRAME_HEIGHT));



    cv::VideoWriter outputVideo; // Open the output
    inputVideo->set(CV_CAP_PROP_POS_FRAMES,0);

    for(int i=0;i<numSlices+1;i++) {
        const std::string NAME = filename.left(pAt-1).toStdString().c_str()+std::to_string(i)+".avi";   // Form the new name
        std::cout << NAME << std::endl;
        outputVideo.open(NAME, CV_FOURCC('x','2','6','4'), inputVideo->get(CV_CAP_PROP_FPS), S, true);
//        outputVideo.open(NAME, -1, inputVideo->get(CV_CAP_PROP_FPS), S, true);

        if (!outputVideo.isOpened()) {
            QMessageBox msgBox;
            msgBox.setText("Could not open the output video for write!");
            msgBox.exec();
            break;
        }
        cv::Mat src;
        if (i>0) inputVideo->set(CV_CAP_PROP_POS_FRAMES,i*framesPerSlice-(int)fps*2);
        for(int n=0;n<framesPerSlice+(int)fps*2;n++) //Show the image captured in the window and repeat
        {

            *inputVideo >> src;              // read
            if (src.empty()) return;         // check if at end
            outputVideo << src;
        }
    }
    std::cout << "Finished writing" << std::endl;
}

#include "../../include/fish_annotator/video_slicer/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_slicer
