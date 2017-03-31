#ifndef NEWS_SERVER_H
#define NEWS_SERVER_H

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

class NewsServer {
	private:
		void list_newsgroups(std::shared_ptr<Connection> conn);
};

#endif
