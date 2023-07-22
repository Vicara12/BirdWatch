#include "videoinput.h"

VideoInput::VideoInput () : inited(false) {};


VideoInput::~VideoInput ()
{
  if (inited) {
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    avcodec_free_context(&av_codec_ctx);
    sws_freeContext(sws_scaler_ctx);
    av_packet_free(&av_packet);
    av_frame_free(&av_frame);
  }
}


bool VideoInput::init (std::string source, int &width, int &height)
{
  inited = false;
  av_format_ctx = avformat_alloc_context();
  if (not av_format_ctx)
    return false;
  if (avformat_open_input(&av_format_ctx, source.c_str(), NULL, NULL)) {
    std::cout << "ERROR: could not open video source \"" << source << "\"" << std::endl;
    return false;
  }

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

  av_frame = av_frame_alloc();
  if (not av_frame)
    return false;
  av_packet = av_packet_alloc();
  if (not av_packet)
    return false;

  if (not searchFrame())
    return false;
  first_frame = true;

  width = this->width = av_frame->width;
  height = this->height = av_frame->height;
  sws_scaler_ctx = sws_getContext(width, height, av_codec_ctx->pix_fmt,
                                  width, height, AV_PIX_FMT_RGB0,
                                  SWS_BILINEAR, NULL, NULL, NULL);
  if (not sws_scaler_ctx)
    return false;

  return inited = true;
}


bool VideoInput::loadFrame (unsigned char **data)
{
  if (not inited) {
    std::cout << "ERROR: tried to render a non-initialized "
              << "video input" << std::endl;
    return false;
  }

  // the first frame is loaded at init to check video resolution, so there is
  // no need to load it again
  if (not first_frame) {
    if (not searchFrame())
      return false;
  } else
    first_frame = false;

  *data = new unsigned char[width * height * 4];
  unsigned char *dest[4] = {*data, NULL, NULL, NULL};
  int dest_linesize[4] = {width*4, 0, 0, 0};
  sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, height, dest, dest_linesize);

  return true;
}



bool VideoInput::searchFrame ()
{
  while (av_read_frame(av_format_ctx, av_packet) >= 0) {
    if (av_packet->stream_index != video_stream_index)
      continue;
    int response = avcodec_send_packet(av_codec_ctx, av_packet);
    if (response < 0) {
      //printf("ERROR: could not decode video packet: %s\n", av_err2str(response));
      return false;
    }
    response = avcodec_receive_frame(av_codec_ctx, av_frame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
      continue;
    else if (response < 0) {
      //printf("ERROR: could not decode video packet: %s\n", av_err2str(response));
      return false;
    }

    av_packet_unref(av_packet);
    break;
  }

  return true;
}
