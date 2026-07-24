#pragma once
#include <FL/Fl_Text_Editor.H>

class Editor : public Fl_Text_Editor {
public:
  Editor() : Editor(300, 30, 980, 690) {};
  Editor(int x, int y, int w, int h);
  ~Editor();
};
