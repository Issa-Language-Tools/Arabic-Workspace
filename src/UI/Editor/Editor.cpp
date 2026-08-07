#include "Editor.hpp"

Editor::Editor(int x, int y, int w, int h, Database* db)
  : Fl_Group(x, y, w, h)
  , db(db)
{
  begin();
  // styling
  box(FL_FLAT_BOX);
  color(fl_rgb_color(40, 40, 40), fl_rgb_color(220, 220, 0));

  row = new Fl_Flex(0, 0, w, h, Fl_Flex::HORIZONTAL);

  row->end();
  end();
}

Editor::~Editor() {}
