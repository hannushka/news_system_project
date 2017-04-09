#ifndef NEWS_CLIENT_H
#define NEWS_CLIENT_H

#include <memory>
#include <string>
#include "connection.h"

class NewsClient {
public:
  NewsClient();
  void list_newsgroups();
  void create_newsgroup(std::string name);
  void delete_newsgroup(int id);
  void list_articles(int id);
  void read_article(int article_id, int news_group_id);
  void set_conn(std::shared_ptr<Connection> conn);

  int read_number();
  void write_number(int value);
private:
  std::shared_ptr<Connection> conn;
};

#endif
