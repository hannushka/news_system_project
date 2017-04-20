#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include <map>

class Article {
public:
  Article(unsigned int id, std::string title,
    std::string author, std::string text);
  std::string get_title();
  std::string get_author();
  std::string get_text();
private:
  unsigned int id;
  std::string title;
  std::string author;
  std::string text;
};

#endif
