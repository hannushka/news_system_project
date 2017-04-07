#include "news_server.h"

using namespace std;

void NewsServer::list_newsgroups(std::shared_ptr<Connection> conn) {
	conn->write(Protocol::ANS_LIST_NG);
	string msg = "hejsan";
	for (char c : msg)
		conn->write(c);
	conn->write(Protocol::ANS_END);
}
