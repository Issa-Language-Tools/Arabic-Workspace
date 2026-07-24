#pragma once
#include <FL/Fl_Box.H>

class TitleBar : public Fl_Box {
  public:
    TitleBar() : TitleBar(300, 0, 980, 30, "Arabic Workspace") {};
    TitleBar(int x, int y, int w, int h, const char* L);
};
