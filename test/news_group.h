#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include <string>
#include <unordered_map>

class NewsGroup {
public:
private:
  int id;
  std::string name;
  unordered_map<Article> articles;
};

#endif
