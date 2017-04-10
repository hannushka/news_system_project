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
  void delete_newsgroup(unsigned int id);
  void list_articles(unsigned int id);
  void read_article(unsigned int article_id, unsigned int news_group_id);
  void set_conn(std::shared_ptr<Connection> conn);

  unsigned int read_number();
  void write_number(unsigned int value);
private:
  std::shared_ptr<Connection> conn;
};

#endif
