#pragma once
#include <FL/Fl_Box.H>

class TitleBar : public Fl_Box {
  public:
    TitleBar(int x, int y, int w, int h, const char* L);
};
