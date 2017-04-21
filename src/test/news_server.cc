#include "news_server.h"

using namespace std;

NewsServer::NewsServer(bool in_memory) {
	//controller is put on heap, since the program does not know
	//wheter it is an in_memory controller or a disk version
	//until runtime
	if (in_memory)
		controller = unique_ptr<MemServerController>(new MemServerController());
	else
		controller = unique_ptr<DiskServerController>(new DiskServerController());
}

void NewsServer::run(Server& server) {
	cout << "server is running" << endl;
  while (true) {
		auto conn = server.waitForActivity();
    controller->set_conn(conn);
		if (conn != nullptr) {
			try {
				char c = conn->read();
				switch (c) {
					case Protocol::COM_LIST_NG:
          if (conn->read() == Protocol::COM_END)
            controller->list_newsgroups();
          break;

          case Protocol::COM_CREATE_NG:
          c = conn->read();
          if (c == Protocol::PAR_STRING) { //always true?
            int nbr_chars = controller->read_number();
            string msg;
            for (int i = 0; i != nbr_chars; ++i)
              msg += conn->read();
            if (conn->read() == Protocol::COM_END)
              controller->create_newsgroup(msg);
          }
					break;

          case Protocol::COM_DELETE_NG:
          if (conn->read() == Protocol::PAR_NUM) {
            unsigned int id = controller->read_number();
            if (conn->read() == Protocol::COM_END)
              controller->delete_newsgroup(id);
          }
          break;

          case Protocol::COM_LIST_ART:
          if (conn->read() == Protocol::PAR_NUM) {
            unsigned int id = controller->read_number();
            if (conn->read() == Protocol::COM_END)
              controller->list_articles(id);
          }
          break;

          case Protocol::COM_CREATE_ART:
          if (conn->read() == Protocol::PAR_NUM) {
            unsigned int id = controller->read_number();
            string title;
            string author;
            string text;
            if (conn->read() == Protocol::PAR_STRING) {
              int nbr_chars = controller->read_number();
              for (int i = 0; i != nbr_chars; ++i)
                title += conn->read();
            }

            if (conn->read() == Protocol::PAR_STRING) {
              int nbr_chars = controller->read_number();
              for (int i = 0; i != nbr_chars; ++i)
                author += conn->read();
            }

            if (conn->read() == Protocol::PAR_STRING) {
              int nbr_chars = controller->read_number();
              for (int i = 0; i != nbr_chars; ++i)
                text += conn->read();
            }

            if (conn->read() == Protocol::COM_END)
              controller->create_article(id, title, author, text);
          }
          break;

          case Protocol::COM_DELETE_ART:
          unsigned int ng_id;
          unsigned int art_id;
          if (conn->read() == Protocol::PAR_NUM) {
            ng_id = controller->read_number();
            if (conn->read() == Protocol::PAR_NUM) {
              art_id = controller->read_number();
              if (conn->read() == Protocol::COM_END) {
                controller->delete_article(ng_id, art_id);
              }
            }
          }
          break;

          case Protocol::COM_GET_ART:
          unsigned int news_group_id;
          unsigned int article_id;
          if (conn->read() == Protocol::PAR_NUM) {
            news_group_id = controller->read_number();
            if (conn->read() == Protocol::PAR_NUM) {
              article_id = controller->read_number();
              if (conn->read() == Protocol::COM_END) {
                controller->read_article(news_group_id, article_id);
              }
            }
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

	cout << "press 1 for in memory version, 2 for disk version" << endl;
	string answer;
	cin >> answer;

	try {
		int result = stoi(answer);
		if (result == 1) {
			NewsServer news_server(true);
			news_server.run(server);
		} else if (result == 2) {
			NewsServer news_server(false);
			news_server.run(server);
		} else {
			cout << "neither 1 or 2 was pressed" << endl;
		}
	} catch (std::invalid_argument) {
		cerr << "must enter an integer. Exiting..." << endl;
		exit(1);
	}
}
