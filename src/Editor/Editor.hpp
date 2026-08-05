#pragma once
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>

class Editor : public Fl_Group {
public:
  Editor(int x, int y, int w, int h);
  ~Editor();
private:
  Fl_Flex* row;
};
