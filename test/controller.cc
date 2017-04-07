#include "controller.h"

using namespace std;

Controller::Controller() {
	NewsGroup n(1,"Sverige");
	news_groups.emplace(1, n);
	NewsGroup n2(2,"Utrikes");
	news_groups.emplace(1, n2);
}

void Controller::list_newsgroups(std::shared_ptr<Connection> conn) {
	conn->write(Protocol::ANS_LIST_NG);
	for (auto it = news_groups.begin() ; it != news_groups.end() ; ++it) {
		string msg = (*it).second.get_name();
		for (char c : msg)
			conn->write(c);
		auto tmp = it;
		if (++tmp != news_groups.end())
			conn->write('\n');
	}
	conn->write(Protocol::ANS_END);
}
