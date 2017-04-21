#include "news_group.h"

using namespace std;

NewsGroup::NewsGroup(unsigned int id, std::string name) : id(id), name(name) {

}

string NewsGroup::get_name() {
  return name;
}

bool NewsGroup::add_article(unsigned int id, Article article) {
    //it will always be possible to add a new article
    articles.emplace(id, article);
    return true;
}

bool NewsGroup::remove_article(unsigned int id) {
  auto it = articles.find(id);
  if (it != articles.end()) {
    articles.erase(it);
    return true;
  }
  return false;
}

std::map<unsigned int, Article> NewsGroup::get_articles() {
  return articles;
}
