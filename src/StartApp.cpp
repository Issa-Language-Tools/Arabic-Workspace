#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tile.H>

#include "Panel/Panel.hpp"
#include "Editor/Editor.hpp"
#include "TitleBar/TitleBar.hpp"

int main() {
  Fl::scheme("gleam");
  Fl::set_font(FL_HELVETICA, "Noto Sans");
  Fl_Double_Window win(1280, 720, "Workspace");

  Fl_Tile tile(0, 0, 1280, 720);
  tile.begin();

  Panel left_panel(0, 0, 300, 720);
  left_panel.box(FL_FLAT_BOX);
  left_panel.color(fl_rgb_color(48, 48, 48));
  left_panel.end();

  Fl_Group right_side(300, 0, 980, 720);
  right_side.box(FL_FLAT_BOX);
  right_side.color(fl_rgb_color(40, 40, 40));

  TitleBar titlebar;
  Editor editor;

  right_side.end();
  tile.end();
  win.end();

  win.resizable(tile);
  win.show();

  return Fl::run();
}
