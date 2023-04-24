#ifndef SCREENMESSAGES_H_
#define SCREENMESSAGES_H_

#include "textrender.h"
#include <map>

class ScreenMessages
{
public:

  ScreenMessages ();
  void addMessage (int txt_id);
  void setMessageStatus (int txt_id, bool display);

private:

  TextRenderer *text_rndr;
  std::map<int,bool> msg_displayed; // int is text id
};

#endif
