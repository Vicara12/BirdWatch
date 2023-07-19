#ifndef VIDEOINPUT_H_
#define VIDEOINPUT_H_

#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <inttypes.h>
}

class VideoInput
{
  public:
    VideoInput ();
    ~VideoInput ();

    bool init (std::string source);
    bool loadFrame (int &width, int &height, unsigned char **data);

  private:

    bool inited;
    AVFormatContext *av_format_ctx;
    AVCodecContext *av_codec_ctx;
    int video_stream_index;
};

#endif
