#pragma once
#include <FL/Fl_Box.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>

#include <vector>

#include "Plumbing/Database/Database.hpp"
#include "UI/Editor/NewEntry/EditPopup/EditPopup.hpp"
#include "UI/Theme/Theme.hpp"

class WordList : public Fl_Group {
public:
  WordList(int x, int y, int w, int h, Database* db);
  void populate(const std::vector<Word>& words);

private:
  Database* db;  // read only
  Fl_Scroll* scroll;
  Fl_Flex* body;
  Fl_Flex* list_col;
  std::vector<Word> words;
};
