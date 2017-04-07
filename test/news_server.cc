#include "news_server.h"

using namespace std;

void list_newsgroups(std::shared_ptr<Connection> conn) {
	conn->write(Protocol::ANS_LIST_NG);
	string msg = "hejsan";
	for (char c : msg)
		conn->write(c);
	conn->write(Protocol::ANS_END);
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}

	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				char c = conn->read();
				switch (c) {
					case Protocol::COM_LIST_NG: list_newsgroups(conn);
					break;
					default: conn->write('-');
				}
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}
