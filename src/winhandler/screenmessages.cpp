#include "screenmessages.h"


ScreenMessages::ScreenMessages () :
  text_rndr(TextRenderer::getInstance())
{
}


void ScreenMessages::addMessage (int txt_id)
{
  msg_displayed[txt_id] = true;
}


void ScreenMessages::setMessageStatus (int txt_id, bool display)
{
  if (display != msg_displayed[txt_id]) {
    if (display)
      text_rndr->enableText(txt_id);
    else
      text_rndr->disableText(txt_id);
    msg_displayed[txt_id] = display;
  }
}
