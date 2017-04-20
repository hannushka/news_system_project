#include "client_controller.h"
#include <iostream>

using namespace std;

ClientController::ClientController() : Controller() {
}

void ClientController::list_newsgroups() {
	conn->write(Protocol::COM_LIST_NG);
	conn->write(Protocol::COM_END);

	char c = conn->read();
	if (c == Protocol::ANS_LIST_NG) {
		string msg;
		c = conn->read();
		if (c == Protocol::PAR_NUM) {
			int nbr_ng = read_number();
			for (int i = 0; i != nbr_ng; ++i) {
				if (conn->read() == Protocol::PAR_NUM) {
					unsigned int id = read_number();
					if (conn->read() == Protocol::PAR_STRING) {
						int str_len = read_number();
						for (int j = 0; j != str_len; ++j)
							msg += conn->read();
						msg += ", id = " + to_string(id);;
						msg += '\n';
					}
				}
			}
		}
		cout << msg;
		if (conn->read() != Protocol::ANS_END)
			cerr << "did not reach end" << endl;
	}
}

void ClientController::create_newsgroup(std::string name) {
	conn->write(Protocol::COM_CREATE_NG);
	conn->write(Protocol::PAR_STRING);
	write_number(name.size());
	write_string(name);
	conn->write(Protocol::COM_END);

	char c = conn->read();
	if (c == Protocol::ANS_CREATE_NG) {
		c = conn->read();
		if (c == Protocol::ANS_ACK) {
			cout << name << " was added successfully" << endl;
		} else if (c == Protocol::ANS_NAK) {
			c = conn->read();
			if (c == Protocol::ERR_NG_ALREADY_EXISTS) {
				cout << name << " already exists" << endl;
			}
		}
		if (conn->read() != Protocol::ANS_END)
			cerr << "did not reach end" << endl;
	}
}

void ClientController::delete_newsgroup(unsigned int id) {
	conn->write(Protocol::COM_DELETE_NG);
	conn->write(Protocol::PAR_NUM);
	write_number(id);
	conn->write(Protocol::COM_END);

	char c = conn->read();
	if (c == Protocol::ANS_DELETE_NG) {
		c = conn->read();
		if (c == Protocol::ANS_ACK) {
			cout << "newsgroup with id = " << id
			<< " was deleted successfully" << endl;
		} else if (c == Protocol::ANS_NAK) {
			c = conn->read();
			if (c == Protocol::ERR_NG_DOES_NOT_EXIST) {
				cout << "newsgroup with id = " << id
				<< " does not exist" << endl;
			}
		}
	if (conn->read() != Protocol::ANS_END)
		cerr << "did not reach end" << endl;
	}
}

void ClientController::list_articles(unsigned int id) {
	conn->write(Protocol::COM_LIST_ART);
	conn->write(Protocol::PAR_NUM);
	write_number(id);
	conn->write(Protocol::COM_END);

	char c = conn->read();
	if (c == Protocol::ANS_LIST_ART) {
		string msg;
		c = conn->read();
		if (c == Protocol::ANS_ACK) {
			if (conn->read() == Protocol::PAR_NUM) {
				unsigned int nbr_art = read_number();
				for (unsigned int i = 0; i != nbr_art; ++i) {
					if (conn->read() == Protocol::PAR_NUM) {
						unsigned int art_id = read_number();
						if (conn->read() == Protocol::PAR_STRING) {
							unsigned int str_len = read_number();
							for (unsigned int j = 0; j != str_len; ++j)
								msg += conn->read();
							msg += ", id = " + to_string(art_id);;
							msg += '\n';
						}
					}
				}
			}
		} else if (c == Protocol::ANS_NAK) {
			c = conn->read();
			if (c == Protocol::ERR_NG_DOES_NOT_EXIST) {
				cout << "newsgroup with id = " << id
				<< " does not exist" << endl;
			}
		}
	cout << msg;
	if (conn->read() != Protocol::ANS_END)
		cerr << "did not reach end" << endl;
	}
}

void ClientController::create_article(unsigned int id, string title,
	string author, string text) {

	conn->write(Protocol::COM_CREATE_ART);
	conn->write(Protocol::PAR_NUM);
	write_number(id);

	conn->write(Protocol::PAR_STRING);
	write_number(title.size());
	write_string(title);

	conn->write(Protocol::PAR_STRING);
	write_number(author.size());
	write_string(author);

	conn->write(Protocol::PAR_STRING);
	write_number(text.size());
	write_string(text);

	conn->write(Protocol::COM_END);

	//Read
	char c = conn->read();
	if (c == Protocol::ANS_CREATE_ART) {
		c = conn->read();
		if (c == Protocol::ANS_ACK) {
			cout << "The article " << title << " was added successfully"
			 << endl;
		} else if (c == Protocol::ANS_NAK) {
			if (conn->read() == Protocol::ERR_NG_DOES_NOT_EXIST) {
				cout << "newsgroup with id " << id << " does not exist"
				<< endl;
			}
		}
		if (conn->read() != Protocol::ANS_END)
			cerr << "did not reach end" << endl;
	}
}

void ClientController::delete_article(unsigned int news_group_id,
	unsigned int article_id) {
	conn->write(Protocol::COM_DELETE_ART);

	conn->write(Protocol::PAR_NUM);
	write_number(news_group_id);
	conn->write(Protocol::PAR_NUM);
	write_number(article_id);

	conn->write(Protocol::COM_END);

	//Read
	char c = conn->read();
	if (c == Protocol::ANS_DELETE_ART) {
		c = conn->read();
		if (c == Protocol::ANS_ACK) {
			cout << "article with id " << article_id <<
			" was successfully deleted from newsgroup with id "
			<< news_group_id << endl;
		 } else if (c == Protocol::ANS_NAK) {
			 c = conn->read();
			 if (c == Protocol::ERR_NG_DOES_NOT_EXIST) {
				 cout << "newsgroup with id " << news_group_id <<
				 " does not exist" << endl;
			 } else if (c == Protocol::ERR_ART_DOES_NOT_EXIST) {
				 cout << "article with id " << article_id <<
				 " does not exist" << endl;
			 }
		 }
	 }
	 if (conn->read() != Protocol::ANS_END)
		 cerr << "did not reach end" << endl;
}

void ClientController::read_article(unsigned int news_group_id,
	unsigned int article_id) {
	conn->write(Protocol::COM_GET_ART);

	conn->write(Protocol::PAR_NUM);
	write_number(news_group_id);
	conn->write(Protocol::PAR_NUM);
	write_number(article_id);

	conn->write(Protocol::COM_END);

	//Read
	char c = conn->read();
	if (c == Protocol::ANS_GET_ART) {
		c = conn->read();
		if (c == Protocol::ANS_ACK) {
			if (conn->read() == Protocol::PAR_STRING) {
				int str_len = read_number();
				string title;
				for (int i = 0; i != str_len; ++i) {
					title += conn->read();
				}
				cout << "Article: " << title << endl;
			}

			if (conn->read() == Protocol::PAR_STRING) {
				int str_len = read_number();
				string author;
				for (int i = 0; i != str_len; ++i) {
					author += conn->read();
				}
				cout << "Author: " << author << endl;
			}

			if (conn->read() == Protocol::PAR_STRING) {
				int str_len = read_number();
				string text;
				for (int i = 0; i != str_len; ++i) {
					text += conn->read();
				}
				cout << "Text: " << endl << text << endl;
			}
		 } else if (c == Protocol::ANS_NAK) {
			 c = conn->read();
			 if (c == Protocol::ERR_NG_DOES_NOT_EXIST) {
				 cout << "newsgroup with id " << news_group_id <<
				 " does not exist" << endl;
			 } else if (c == Protocol::ERR_ART_DOES_NOT_EXIST) {
				 cout << "article with id " << article_id <<
				 " does not exist" << endl;
			 }
		 }
	 }
	 if (conn->read() != Protocol::ANS_END)
		 cerr << "did not reach end" << endl;
}
