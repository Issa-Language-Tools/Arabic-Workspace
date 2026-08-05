#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tile.H>

#include "Panel/Panel.hpp"
#include "Editor/Editor.hpp"
#include "TitleBar/TitleBar.hpp"

int main() {
  Fl::scheme("gleam");
  Fl::set_font(FL_HELVETICA, "Noto Sans");
  const int WIN_X = 1280;
  const int WIN_Y = 720;
  Fl_Double_Window win(WIN_X, WIN_Y, "Workspace");

  Fl_Tile tile(0, 0, WIN_X, WIN_Y);
  tile.begin();

  Panel* left_panel = new Panel(0,0, 300, WIN_Y);
  left_panel->begin();
  left_panel->color(fl_rgb_color(48, 48, 48));
  left_panel->end();


  Panel* right_panel = new Panel(300,0,WIN_X-300, WIN_Y);
  right_panel->begin();
  right_panel->color(fl_rgb_color(55,55,55));
  TitleBar* titlebar = new TitleBar(300, 0, WIN_X-300, 30, "Arabic Workspace");
  Editor* editor = new Editor(300, 30, WIN_X-300, WIN_Y-30);

  right_panel->end();
  tile.end();
  win.end();

  win.resizable(tile);
  win.show();

  return Fl::run();
}
