/// @file
/// @brief Defines class for playing video.

#ifndef VIDEO_ANNOTATOR_PLAYER_H
#define VIDEO_ANNOTATOR_PLAYER_H

#include <string>
#include <memory>
#include <atomic>
#include <map>

#include <boost/bimap.hpp>

#include <QImage>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h>
#include <libswscale/swscale.h>
}

namespace fish_annotator { namespace video_annotator {

/// Class for playing video.
class Player : public QObject {	
    Q_OBJECT
public:
    /// Constructor.
    explicit Player();

    /// Destructor.
    ~Player();
public slots:
    /// Plays the video.
    void play();
    
    /// Stops the video.
    void stop();

    /// Loads the video.
    ///
    /// @param filename Path to video.
    void loadVideo(QString filename);

    /// Increases the speed of the video by a factor of two.
    void speedUp();

    /// Decreases the speed of the video by a factor of two.
    void slowDown();

    /// Sets position to specified frame.
    ///
    /// @param frame Frame to seek to.
    void setFrame(qint64 frame);

    /// Sets position to next frame.
    void nextFrame();

    /// Sets position to previous frame.
    void prevFrame();
signals:
    /// Emitted when a frame is ready to display.
    //
    /// @param image Captured image.
    /// @param frame Frame number that corresponds to this image.
    void processedImage(QImage image, qint64 frame);

    /// Emitted when duration changes.
    ///
    /// @param duration New video duration.
    void durationChanged(qint64 duration);

    /// Emitted when playback rate changes.
    ///
    /// @param rate Playback rate (fps).
    void playbackRateChanged(double rate);

    /// Emitted when video resolution changes.
    ///
    /// @param width Video width.
    /// @param height Video height.
    void resolutionChanged(qint64 width, qint64 height);

    /// Emitted when play/pause state changed.
    ///
    /// @param stopped True if stopped, false otherwise.
    void stateChanged(bool stopped);

    /// Emitted when media load starts.
    ///
    /// @param max_progress Maximum progress.
    void mediaLoadStart(int max_progress);

    /// Emitted when media load progresses.
    ///
    /// @param progress Load progress.
    void loadProgress(int progress);

    /// Emitted when new media is loaded.
    ///
    /// @param video_path Path to loaded video file.
    /// @param native_rate Native playback rate of the video.
    void mediaLoaded(QString video_path, qreal native_rate);

    /// Emitted on error.
    ///
    /// @param err Error message.
    void error(QString err);
private:
    /// Path to loaded video.
    QString video_path_;

    /// Native frame rate of loaded video.
    double frame_rate_;

    /// True if player is stopped, false otherwise.
    bool stopped_;

    /// Stores most recent image.
    QImage image_;

    /// Current playback rate.
    double current_speed_;

    /// Codec context.
    AVCodecContext *codec_context_;

    /// Format context.
    AVFormatContext *format_context_;

    /// Packet.
    AVPacket packet_;

    /// Index of video stream.
    int stream_index_;

    /// Most recent frame.
    AVFrame *frame_;

    /// For converting decoded frame to RGB.
    AVFrame *frame_rgb_;

    /// Conversion context.
    SwsContext *sws_context_;

    /// Delay between frames in microseconds.
    double delay_;

    /// Last decoded frame.
    qint64 dec_frame_;

    /// Last requested frame.
    qint64 req_frame_;

    /// Map between frame index and decompression timestamp.
    boost::bimap<qint64, qint64> seek_map_;

    /// Frame buffer.
    std::map<qint64, QImage> frame_buffer_;

    /// Mutex for grabbing frames.
    QMutex frame_mutex_;

    /// True when buffering.
    std::atomic<bool> buffering_;

    /// Wait condition for deletion.
    QWaitCondition condition_;

    /// Processes a single frame.
    void getOneFrame();

    /// Sets the current frame.
    ///
    /// @param frame_num Set to this frame. Bounded by this function.
    void setCurrentFrame(qint64 frame_num);

    /// Buffers frames.
    ///
    /// @param frame_num Buffer up to this frame number.
    /// @param wait Number of usec to wait between decoding frames.
    void buffer(qint64 frame_num, qint64 wait = 0);

    /// Waits for specified time while allowing events to process.
    void processWait(qint64 usec);

    /// Reinitializes the player.
    void reinit();
};

}} // namespace fish_annotator::video_annotator

#endif // VIDEO_ANNOTATOR_PLAYER_H
