#include "Editor.hpp"

#include <FL/Fl_Text_Buffer.H>

Editor::Editor(int x, int y, int w, int h)
  : Fl_Text_Editor(x, y, w, h) {
  // initialize the buffer
  Fl_Text_Buffer* buf = new Fl_Text_Buffer();
  buffer(buf);

  // styling
  box(FL_FLAT_BOX);
  color(fl_rgb_color(40, 40, 40), fl_rgb_color(220, 220, 0));
  textsize(20);
  cursor_style(Editor::NORMAL_CURSOR);
  cursor_color(FL_WHITE);
  textcolor(fl_rgb_color(220, 220, 220));
  buf->text("");
}

Editor::~Editor() {
  delete buffer();
}
