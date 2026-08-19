#include "NewEntry.hpp"

#include <FL/Fl_Button.H>

#include <iostream>

#define COL_W 400

NewEntry::NewEntry(int x, int y, int w, int h, Database* db) : Fl_Group(x, y, w, h), db(db) {
  Theme::setup_boxtype();

  arabic_buf = new Fl_Text_Buffer();
  english_buf = new Fl_Text_Buffer();

  Fl_Flex* col = new Fl_Flex(0, 0, w, h, Fl_Flex::VERTICAL);
  col->gap(0);

  Fl_Flex* header = new Fl_Flex(0, 0, w, Theme::HEADER_H, Fl_Flex::HORIZONTAL);
  header->gap(4);
  auto* arabic_label = new Fl_Box(0, 0, 0, 0);
  Theme::style_header(arabic_label, "عربي");
  auto* english_label = new Fl_Box(0, 0, 0, 0);
  Theme::style_header(english_label, "English");
  header->end();
  header->fixed(arabic_label, COL_W);
  header->fixed(english_label, COL_W);

  Fl_Flex* inputs = new Fl_Flex(0, 0, w, h - Theme::HEADER_H, Fl_Flex::HORIZONTAL);
  inputs->gap(4);

  Arabic = new Fl_Text_Editor(0, 0, 0, 0);
  Theme::style_editor(Arabic, arabic_buf);

  English = new Fl_Text_Editor(0, 0, 0, 0);
  Theme::style_editor(English, english_buf);

  Fl_Button* save_button = new Fl_Button(0, 0, 0, 0);
  Theme::style_button(save_button, "Save");
  save_button->callback([](Fl_Widget*, void* data) {
    static_cast<NewEntry*>(data)->save();
  }, this);

  Fl_Button* edit_button = new Fl_Button(0, 0, 0, 0);
  Theme::style_button(edit_button, "Edit");
  edit_button->callback([](Fl_Widget*, void* data) {
    NewEntry* self = static_cast<NewEntry*>(data);
    EditPopup popup(self->newWord);
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
    self->save();
  }, this);

  inputs->end();
  inputs->fixed(Arabic, COL_W);
  inputs->fixed(English, COL_W);
  inputs->fixed(save_button, 60);
  inputs->fixed(edit_button, 60);

  col->fixed(header, Theme::HEADER_H);
  col->end();
  end();
}

void NewEntry::clear() {
  arabic_buf->text("");
  english_buf->text("");
  Arabic->take_focus();
}

void NewEntry::save() {
  newWord.english = english_buf->text();
  newWord.arabic = arabic_buf->text();
  std::cout << newWord.english << " : " << newWord.arabic << '\n';
  clear();
}
