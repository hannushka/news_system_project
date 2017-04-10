#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include <unordered_map>

class Article {
public:
  Article(unsigned int id, std::string title,
    std::string author, std::string text);
  std::string get_title();
private:
  unsigned int id;
  std::string title;
  std::string author;
  std::string text;
};

#endif
