#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>

namespace Theme {

// palette
inline const Fl_Color BG_EDITOR    = fl_rgb_color(40, 40, 40);
inline const Fl_Color BG_INPUT     = fl_rgb_color(50, 50, 50);
inline const Fl_Color BG_HEADER    = fl_rgb_color(55, 55, 55);
inline const Fl_Color TEXT_PRIMARY = fl_rgb_color(245, 245, 245);
inline const Fl_Color BORDER       = fl_rgb_color(200, 200, 200);

// font sizes
inline const int TEXT_SIZE = 20;
inline const int HEADER_H  = 20;

// styling helpers
void setup_boxtype();
void style_editor(Fl_Text_Editor* e, Fl_Text_Buffer* buf);
void style_input(Fl_Input* in);
void style_button(Fl_Button* b, const char* text);
void style_header(Fl_Box* box, const char* text);

}  // namespace Theme
