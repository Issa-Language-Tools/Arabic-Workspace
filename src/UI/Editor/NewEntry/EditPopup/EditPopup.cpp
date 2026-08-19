#include "EditPopup.hpp"
#include <FL/Fl_Flex.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

#include "UI/Theme/Theme.hpp"

namespace {
const int ROW_H = 40;
const int LABEL_W = 100;
}

EditPopup::EditPopup(const Word& word)
  : Fl_Window(420, 260, "Edit Word") {

  color(Theme::BG_EDITOR);

  Fl_Flex* col = new Fl_Flex(0, 0, 420, 260, Fl_Flex::VERTICAL);
  col->gap(4);
  col->margin(8);

  auto add_row = [&](const char* label, Fl_Input*& out, const std::string& val) {
    Fl_Flex* row = new Fl_Flex(0, 0, 420, ROW_H, Fl_Flex::HORIZONTAL);
    row->gap(4);
    Fl_Box* lbl = new Fl_Box(0, 0, LABEL_W, ROW_H);
    Theme::style_header(lbl, label);
    out = new Fl_Input(0, 0, 0, ROW_H);
    Theme::style_input(out);
    out->value(val.c_str());
    row->fixed(lbl, LABEL_W);
    row->end();
  };

  add_row("English", inp_english, word.english);
  add_row("Arabic", inp_arabic, word.arabic);
  add_row("Source", inp_source, "");
  add_row("Dialect", inp_dialect, "");
  add_row("Wordform", inp_wordform, "");

  Fl_Flex* btn_row = new Fl_Flex(0, 0, 420, ROW_H, Fl_Flex::HORIZONTAL);
  Fl_Button* save = new Fl_Button(0, 0, 0, ROW_H);
  Fl_Button* cancel = new Fl_Button(0, 0, 0, ROW_H);
  Theme::style_button(save, "Save Edit");
  Theme::style_button(cancel, "Cancel");
  btn_row->fixed(save, 120);
  btn_row->fixed(cancel, 120);
  btn_row->end();

  col->end();

  save->callback([](Fl_Widget*, void* data) {
    static_cast<EditPopup*>(data)->do_callback();
  }, this);

  cancel->callback([](Fl_Widget* w, void*) {
    w->window()->hide();
  });

  end();
  set_modal();
}

Word EditPopup::word() const {
  Word w;
  w.english = inp_english->value();
  w.arabic = inp_arabic->value();
  w.source = inp_source->value();
  w.dialect = inp_dialect->value();
  w.wordform = inp_wordform->value();
  return w;
}
