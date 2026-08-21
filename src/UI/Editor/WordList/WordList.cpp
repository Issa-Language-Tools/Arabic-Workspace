#include "WordList.hpp"

#include <FL/Fl_Button.H>

#include <iostream>
#include <vector>

#define ROW_H 40
#define COL_W 300
#define COLS_GAP 4
#define ROW_W (COL_W + COLS_GAP + COL_W + COLS_GAP + COL_W + COLS_GAP + 60)

namespace {
void header_label(Fl_Box* box, const char* text) {
  box->box(FL_NO_BOX);
  box->labelcolor(Theme::TEXT_PRIMARY);
  box->labelsize(Theme::TEXT_SIZE);
  box->labelfont(FL_HELVETICA_BOLD);
  box->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box->label(text);
}
}

WordList::WordList(int x, int y, int w, int h, Database* db) : Fl_Group(x, y, w, h), db(db) {
  Theme::setup_boxtype();

  Fl_Flex* col = new Fl_Flex(0, 0, w, h, Fl_Flex::VERTICAL);
  col->gap(4);

  scroll = new Fl_Scroll(0, 0, w, h);
  scroll->type(Fl_Scroll::BOTH);
  scroll->box(FL_NO_BOX);
  scroll->color(Theme::BG_EDITOR);
  scroll->scrollbar_size(8);
  scroll->begin();

  body = new Fl_Flex(0, 0, ROW_W, ROW_H + COLS_GAP, Fl_Flex::VERTICAL);
  body->gap(COLS_GAP);

  Fl_Flex* header_row = new Fl_Flex(0, 0, ROW_W, ROW_H, Fl_Flex::HORIZONTAL);
  header_row->gap(COLS_GAP);
  auto* arabic_label = new Fl_Box(0, 0, 0, 0);
  header_label(arabic_label, "Arabic");
  auto* english_label = new Fl_Box(0, 0, 0, 0);
  header_label(english_label, "English");
  auto* source_label = new Fl_Box(0, 0, 0, 0);
  header_label(source_label, "Source");
  auto* edit_spacer = new Fl_Box(0, 0, 0, 0);
  header_label(edit_spacer, "");
  header_row->fixed(arabic_label, COL_W);
  header_row->fixed(english_label, COL_W);
  header_row->fixed(source_label, COL_W);
  header_row->fixed(edit_spacer, 60);
  header_row->end();
  body->fixed(header_row, ROW_H);

  list_col = new Fl_Flex(0, 0, ROW_W, 0, Fl_Flex::VERTICAL);
  list_col->box(FL_FLAT_BOX);
  list_col->color(Theme::BG_EDITOR);
  list_col->gap(COLS_GAP);
  list_col->end();

  body->end();
  scroll->end();

  Theme::style_scrollbar(&scroll->scrollbar);
  Theme::style_scrollbar(&scroll->hscrollbar);

  col->fixed(scroll, h);
  col->end();
  resizable(col);
  end();
}

void WordList::populate(const std::vector<Word>& new_words) {
  words = new_words;

  list_col->clear();
  list_col->begin();

  std::vector<Fl_Flex*> rows;
  for (size_t i = 0; i < words.size(); ++i) {
    const Word& word = words[i];

    Fl_Flex* row = new Fl_Flex(0, 0, 0, 0, Fl_Flex::HORIZONTAL);
    row->gap(COLS_GAP);
    rows.push_back(row);

    auto* arabic_box = new Fl_Box(0, 0, 0, 0);
    Theme::style_header(arabic_box, word.arabic.c_str());

    auto* english_box = new Fl_Box(0, 0, 0, 0);
    Theme::style_header(english_box, word.english.c_str());

    auto* source_box = new Fl_Box(0, 0, 0, 0);
    Theme::style_header(source_box, word.source.c_str());

    Fl_Button* edit_button = new Fl_Button(0, 0, 0, 0);
    Theme::style_button(edit_button, "Edit");
    edit_button->user_data(&words[i]);
    edit_button->callback([](Fl_Widget*, void* data) {
      Word* cur = static_cast<Word*>(data);
      EditPopup popup(*cur);
      popup.set_modal();
      popup.show();
      while (popup.shown()) Fl::wait();
      Word w = popup.word();
      std::cout << "id=" << w.id
                << " english=" << w.english
                << " arabic=" << w.arabic
                << " source=" << w.source
                << " dialect=" << w.dialect
                << " wordform=" << w.wordform << '\n';
    });

    row->fixed(arabic_box, COL_W);
    row->fixed(english_box, COL_W);
    row->fixed(source_box, COL_W);
    row->fixed(edit_button, 60);
    row->end();
  }

  for (Fl_Flex* r : rows) {
    list_col->fixed(r, ROW_H);
  }

  list_col->end();

  size_t n = words.size();
  int content_h = (int)n * ROW_H;
  if (n > 1) content_h += (int)(n - 1) * list_col->gap();
  list_col->size(ROW_W, content_h);
  body->size(ROW_W, ROW_H + COLS_GAP + content_h);

  scroll->scroll_to(0, 0);
  scroll->redraw();
  redraw();
}
