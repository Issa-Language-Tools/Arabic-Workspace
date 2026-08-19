#pragma once
#include <FL/Fl_Box.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>

#include "EditPopup/EditPopup.hpp"
#include "Plumbing/Database/Database.hpp"
#include "UI/Theme/Theme.hpp"

class NewEntry : public Fl_Group {
public:
  NewEntry(int x, int y, int w, int h, Database* db);
  void clear();  // clear fields
  void save();   // print current editors
private:
  Database* db;
  Word newWord;
  Fl_Text_Editor* Arabic;
  Fl_Text_Editor* English;
  Fl_Text_Buffer* arabic_buf;
  Fl_Text_Buffer* english_buf;
};
