#include "Theme.hpp"
#include <FL/fl_draw.H>

static void white_border_box(int x, int y, int w, int h, Fl_Color bg) {
  Fl::set_box_color(bg);
  fl_rectf(x, y, w, h);
  fl_color(Theme::BORDER);
  fl_rect(x, y, w, h);
}

void Theme::setup_boxtype() {
  Fl::set_boxtype(FL_FREE_BOXTYPE, white_border_box, 6, 4, 12, 8);
}

void Theme::style_editor(Fl_Text_Editor* e, Fl_Text_Buffer* buf) {
  e->buffer(buf);
  e->box(FL_FREE_BOXTYPE);
  e->color(BG_INPUT);
  e->textsize(TEXT_SIZE);
  e->textcolor(TEXT_PRIMARY);
  e->cursor_color(FL_WHITE);
  e->cursor_style(Fl_Text_Display::NORMAL_CURSOR);
  e->wrap_mode(Fl_Text_Display::WRAP_NONE, 0);
  e->tab_nav(1);
}

void Theme::style_input(Fl_Input* in) {
  in->box(FL_FREE_BOXTYPE);
  in->color(BG_INPUT);
  in->textcolor(TEXT_PRIMARY);
  in->textsize(TEXT_SIZE);
  in->cursor_color(FL_WHITE);
}

void Theme::style_button(Fl_Button* b, const char* text) {
  b->box(FL_FREE_BOXTYPE);
  b->color(BG_HEADER);
  b->labelcolor(FL_WHITE);
  b->labelfont(FL_HELVETICA_BOLD);
  b->labelsize(TEXT_SIZE);
  b->label(text);
}

void Theme::style_header(Fl_Box* box, const char* text) {
  box->box(FL_FLAT_BOX);
  box->color(BG_HEADER);
  box->labelcolor(FL_WHITE);
  box->labelsize(TEXT_SIZE);
  box->labelfont(FL_HELVETICA_BOLD);
  box->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  box->label(text);
}
