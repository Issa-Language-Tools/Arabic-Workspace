#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Check_Button.H>
#include <string>

#include "Word.hpp"

class EditPopup : public Fl_Window {
public:
  EditPopup(const Word& word);
  Word word() const;

private:
  Fl_Input* inp_english;
  Fl_Input* inp_arabic;
  Fl_Input* inp_source;
  Fl_Input* inp_dialect;
  Fl_Input* inp_wordform;
  Fl_Check_Button* chk_exported;
};
