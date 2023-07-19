#include "videoinput.h"

VideoInput::VideoInput () {};


VideoInput::~VideoInput ()
{
  if (inited) {
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    avcodec_free_context(&av_codec_ctx);
  }
}


bool VideoInput::init (std::string source)
{
  inited = false;
  av_format_ctx = avformat_alloc_context();
  if (not av_format_ctx)
    return false;
  if (avformat_open_input(&av_format_ctx, source.c_str(), NULL, NULL))
    return false;

  video_stream_index = -1;
  AVCodecParameters *av_codec_params;
  AVCodec *av_codec;
  for (uint i = 0; i < av_format_ctx->nb_streams; i++) {
    av_codec_params = av_format_ctx->streams[i]->codecpar;
    av_codec = avcodec_find_decoder(av_codec_params->codec_id);
    if (not av_codec) {
      continue;
    }
    if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
      video_stream_index = i;
      break;
    }
  }
  if (video_stream_index == -1) {
    std::cout << "ERROR: could not find a video stream inside the file" << std::endl;
    return false;
  }

  av_codec_ctx = avcodec_alloc_context3(av_codec);
  if (not av_codec_ctx)
    return false;
  if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0)
    return false;
  if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0)
    return false;

  return inited = true;
}


bool VideoInput::loadFrame (int &width, int &height, unsigned char **data)
{
  if (not inited) {
    std::cout << "ERROR: tried to render a non-initialized "
              << "video input" << std::endl;
    return false;
  }

  AVFrame *av_frame = av_frame_alloc();
  if (not av_frame)
    return false;
  AVPacket *av_packet = av_packet_alloc();
  if (not av_packet)
    return false;

  while (av_read_frame(av_format_ctx, av_packet) >= 0) {
    if (av_packet->stream_index != video_stream_index)
      continue;
    int response = avcodec_send_packet(av_codec_ctx, av_packet);
    if (response < 0) {
      std::cout << "ERROR: could not decode video packet: "
                << av_err2str(response) << std::endl;
      return false;
    }
    response = avcodec_receive_frame(av_codec_ctx, av_frame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
      continue;
    else if (response < 0) {
      std::cout << "ERROR: could not decode video packet: "
                << av_err2str(response) << std::endl;
      return false;
    }

    av_packet_unref(av_packet);
    break;
  }

  width = av_frame->width;
  height = av_frame->height;
  *data = new unsigned char[width * height * 3];
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      (*data)[y*width*3 + x*3    ] = av_frame->data[0][y * av_frame->linesize[0] + x];
      (*data)[y*width*3 + x*3 + 1] = av_frame->data[0][y * av_frame->linesize[0] + x];
      (*data)[y*width*3 + x*3 + 2] = av_frame->data[0][y * av_frame->linesize[0] + x];
    }
  }

  av_packet_free(&av_packet);
  av_frame_free(&av_frame);
  return true;
}
