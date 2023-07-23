#ifndef VIDEOINPUT_H_
#define VIDEOINPUT_H_

#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <inttypes.h>
}

class VideoInput
{
  public:
    VideoInput ();
    ~VideoInput ();

    bool init (std::string source, int &width, int &height);
    // gives a pointer to the frame data if success or NULL otherwise
    unsigned char * loadFrame ();

  private:

    bool searchFrame ();
    AVPixelFormat convertPixelFormat (AVPixelFormat px);

    bool inited;
    bool first_frame;
    int width, height;
    AVFormatContext *av_format_ctx;
    AVCodecContext *av_codec_ctx;
    AVFrame *av_frame;
    AVPacket *av_packet;
    SwsContext *sws_scaler_ctx;
    int video_stream_index;
    unsigned char *data;
};

#endif
