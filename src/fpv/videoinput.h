#ifndef VIDEOINPUT_H_
#define VIDEOINPUT_H_

#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

class VideoInput
{
  public:
    VideoInput ();
    ~VideoInput ();

    bool init (std::string source, int &width, int &height);
    bool loadFrame (unsigned char **data);

  private:

    bool searchFrame ();

    bool inited;
    bool first_frame;
    int width, height;
    AVFormatContext *av_format_ctx;
    AVCodecContext *av_codec_ctx;
    AVFrame *av_frame;
    AVPacket *av_packet;
    SwsContext *sws_scaler_ctx;
    int video_stream_index;
};

#endif
