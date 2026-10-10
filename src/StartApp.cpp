#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tile.H>

#include "Plumbing/Database/Database.hpp"
#include "UI/Editor/Editor.hpp"
#include "UI/MenuBar/MenuBar.hpp"
#include "UI/Panel/Panel.hpp"
#include "UI/TitleBar/TitleBar.hpp"



int main() {
  Fl::scheme("gleam");
  Fl::set_font(FL_HELVETICA, "Noto Sans");
  const int WIN_X = 1280;
  const int WIN_Y = 720;

  const int TITLE_H = 30;
  const int MENU_H = 50;
  Fl_Double_Window win(WIN_X, WIN_Y, "Workspace");

  Database db;

  Fl_Tile tile(0, 0, WIN_X, WIN_Y);
  tile.begin();

  Panel* left_panel = new Panel(0, 0, 300, WIN_Y);
  left_panel->begin();
  left_panel->color(fl_rgb_color(48, 48, 48));
  left_panel->end();

  Panel* right_panel = new Panel(300, 0, WIN_X - 300, WIN_Y);
  right_panel->begin();
  right_panel->color(fl_rgb_color(55, 55, 55));
  new TitleBar(300, 0, WIN_X - 300, TITLE_H, "Language Workspace");
  new MenuBar(300, TITLE_H, WIN_X - 300, MENU_H, "Test");
  new Editor(300, TITLE_H + MENU_H, WIN_X - 300, WIN_Y - TITLE_H - MENU_H, &db);

  right_panel->end();
  tile.end();
  win.end();

  win.resizable(tile);
  win.show();

  return Fl::run();
}
