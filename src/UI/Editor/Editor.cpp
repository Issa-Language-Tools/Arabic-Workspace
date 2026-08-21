#include "Editor.hpp"

Editor::Editor(int x, int y, int w, int h, Database* db)
  : Fl_Group(x, y, w, h), db(db) {
  begin();
  // styling
  box(FL_FLAT_BOX);
  color(fl_rgb_color(40, 40, 40), fl_rgb_color(220, 220, 0));

  int list_h = h/2 - 40;
  Fl_Flex* WordsRow = new Fl_Flex(x+25, 40, w, list_h, Fl_Flex::HORIZONTAL);
  wordList = new WordList(0, 0, w, list_h, db);

  std::vector<Word> sample;
  Word w1; w1.id = 1; w1.english = "hello"; w1.arabic = "\xD9\x85\xD8\xB1\xD8\xAD\xD8\xA8\xD8\xA7"; w1.source = "book";
  Word w2; w2.id = 2; w2.english = "thank you"; w2.arabic = "\xD8\xB4\xD9\x83\xD8\xB1\xD8\xA7"; w2.source = "convo";
  Word w3; w2.id = 3; w3.english = "you're welcome"; w3.arabic = "\xD8\xB4\xD9\x83\xD8\xB1\xD8\xA7"; w3.source = "convo";
  Word w4; w2.id = 4; w4.english = "thank you"; w4.arabic = "\xD8\xB4\xD9\x83\xD8\xB1\xD8\xA7"; w4.source = "test";
  Word w5; w2.id = 5; w5.english = "thank you"; w5.arabic = "\xD8\xB4\xD9\x83\xD8\xB1\xD8\xA7"; w5.source = "convo";
  Word w6; w2.id = 6; w6.english = "thank you"; w6.arabic = "\xD8\xB4\xD9\x83\xD8\xB1\xD8\xA7"; w6.source = "convo";
  Word w7; w2.id = 7; w7.english = "thank you"; w7.arabic = "\xD8\xB4\xD9\x83\xD8\xB1\xD8\xA7"; w7.source = "book";
  sample.push_back(w1);
  sample.push_back(w2);
  sample.push_back(w3);
  sample.push_back(w4);
  sample.push_back(w5);
  sample.push_back(w6);
  sample.push_back(w7);

  // db.getAllWords()
  wordList->populate(sample);
  WordsRow->end();

  EntryRow = new Fl_Flex(x+50, h/2, w, 55, Fl_Flex::HORIZONTAL);
  new NewEntry(0, 0, w, 55, db);
  EntryRow->end();
  end();
}

Editor::~Editor() {}
