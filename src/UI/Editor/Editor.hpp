#pragma once
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>

#include "NewEntry/NewEntry.hpp"
#include "Plumbing/Database/Database.hpp"

class Editor : public Fl_Group {
public:
  Editor(int x, int y, int w, int h, Database* db);
  ~Editor();

private:
  Database* db;
  Fl_Flex* EntryRow;
};
