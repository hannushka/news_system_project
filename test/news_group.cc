#include "news_group.h"

using namespace std;

NewsGroup::NewsGroup(unsigned int id, std::string name) : id(id), name(name) {

}

string NewsGroup::get_name() {
  return name;
}

bool NewsGroup::add_article(unsigned int id, Article article) {
/*  auto succ = */articles.emplace(id, article);
/*  if (succ.second)
    return true;
  else
    return false;*/
    return true;
}

std::unordered_map<unsigned int, Article> NewsGroup::get_articles() {
  return articles;
}
