#pragma once
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Box.H>

#include "Plumbing/Database/Database.hpp"

class NewEntry : public Fl_Group {
public:
  NewEntry(int x, int y, int w, int h, Database* db);
  void clear();  // clear fields
  void save();   // add word to db
private:
  Database* db;
  Fl_Text_Editor* Arabic;
  Fl_Text_Editor* English;
  Fl_Text_Buffer* arabic_buf;
  Fl_Text_Buffer* english_buf;
};
