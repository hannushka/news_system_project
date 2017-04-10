#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include "article.h"

#include <string>
#include <unordered_map>

class NewsGroup {
public:
  NewsGroup(unsigned int id, std::string name);
  std::string get_name();
  bool add_article(unsigned int id, Article article);
  bool remove_article(unsigned int id);
  std::unordered_map<unsigned int, Article> get_articles();

private:
  unsigned int id;
  std::string name;
  std::unordered_map<unsigned int, Article> articles;
};

#endif
