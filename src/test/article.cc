#include "article.h"

using namespace std;

Article::Article(unsigned int id, std::string title, std::string author,
  std::string text) : id(id), title(title), author(author), text(text) {

}

string Article::get_title() {
  return title;
}

string Article::get_author() {
  return author;
}

string Article::get_text() {
  return text;
}
