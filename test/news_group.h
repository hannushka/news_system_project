#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include "article.h"

#include <string>
#include <unordered_map>

class NewsGroup {
public:
  NewsGroup(int id, std::string name);
  std::string get_name();
private:
  int id;
  std::string name;
  std::unordered_map<int,Article> articles;
};

#endif
