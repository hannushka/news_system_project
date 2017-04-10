#include "news_server.h"

using namespace std;

NewsServer::NewsServer(Controller controller) : controller(controller) {

}

void NewsServer::run(Server& server) {
  while (true) {
		auto conn = server.waitForActivity();
    controller.set_conn(conn);
		if (conn != nullptr) {
			try {
				char c = conn->read();
				switch (c) {
					case Protocol::COM_LIST_NG:
          if (conn->read() == Protocol::COM_END)
            controller.list_newsgroups();
          break;

          case Protocol::COM_CREATE_NG:
          c = conn->read();
          if (c == Protocol::PAR_STRING) { //always true?
            int nbr_chars = controller.read_number();
            string msg;
            for (int i = 0; i != nbr_chars; ++i)
              msg += conn->read();
            if (conn->read() == Protocol::COM_END)
              controller.create_newsgroup(msg);
          }
					break;

          case Protocol::COM_DELETE_NG:
          if (conn->read() == Protocol::PAR_NUM) {
            unsigned int id = controller.read_number();
            if (conn->read() == Protocol::COM_END)
              controller.delete_newsgroup(id);
          }
          break;

          case Protocol::COM_LIST_ART:
          if (conn->read() == Protocol::PAR_NUM) {
            unsigned int id = controller.read_number();
            if (conn->read() == Protocol::COM_END)
              controller.list_articles(id);
          }
          break;

          case Protocol::COM_CREATE_ART:
          cout << "creating article" << endl;
          if (conn->read() == Protocol::PAR_NUM) {
            unsigned int id = controller.read_number();
            string title;
            string author;
            string text;
            if (conn->read() == Protocol::PAR_STRING) {
              int nbr_chars = controller.read_number();
              for (int i = 0; i != nbr_chars; ++i)
                title += conn->read();
            }

            if (conn->read() == Protocol::PAR_STRING) {
              int nbr_chars = controller.read_number();
              for (int i = 0; i != nbr_chars; ++i)
                author += conn->read();
            }

            if (conn->read() == Protocol::PAR_STRING) {
              int nbr_chars = controller.read_number();
              for (int i = 0; i != nbr_chars; ++i)
                text += conn->read();
            }

            if (conn->read() == Protocol::COM_END)
              controller.create_article(id, title, author, text);
          }
          break;

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

int main(int argc, char* argv[]){
  Controller controller;
  NewsServer news_server(controller);
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

  news_server.run(server);

}
