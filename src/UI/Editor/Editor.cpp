#include "Editor.hpp"

Editor::Editor(int x, int y, int w, int h, Database* db)
  : Fl_Group(x, y, w, h), db(db) {
  begin();
  // styling
  box(FL_FLAT_BOX);
  color(fl_rgb_color(40, 40, 40), fl_rgb_color(220, 220, 0));
  

  EntryRow = new Fl_Flex(x+50, h/2, w, 55, Fl_Flex::HORIZONTAL);
  new NewEntry(0, 0, w, 55, db);
  EntryRow->end();
  end();
}

Editor::~Editor() {}
