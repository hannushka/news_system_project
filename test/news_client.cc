#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iterator>
#include "protocol.h"
#include "news_client.h"
#include <sstream>
#include <limits>

using namespace std;

NewsClient::NewsClient() {

}

void NewsClient::list_newsgroups() {
	conn->write(Protocol::COM_LIST_NG);
	conn->write(Protocol::COM_END);

//debugging
	/*char c = conn->read();
	while (c != Protocol::ANS_END) {
		int s = c;
		cout << s << endl;
		c = conn->read();
	}*/

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

void NewsClient::create_newsgroup(std::string name) {
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

void NewsClient::delete_newsgroup(unsigned int id) {
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
			if (c == Protocol::ERR_NG_ALREADY_EXISTS) {
				cout << "newsgroup with id = " << id
				<< " does not exist" << endl;
			}
		}
	if (conn->read() != Protocol::ANS_END)
		cerr << "did not reach end" << endl;
	}
}

void NewsClient::list_articles(unsigned int id) {
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

void NewsClient::create_article(unsigned int id, string title,
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

void NewsClient::set_conn(shared_ptr<Connection> conn) {
	this->conn = conn;
}

void NewsClient::write_string(string msg) {
	for (char c : msg)
	{
		conn->write(c);
	}
}

unsigned int NewsClient::read_number() {
 	unsigned char byte1 = conn->read();
 	unsigned char byte2 = conn->read();
 	unsigned char byte3 = conn->read();
 	unsigned char byte4 = conn->read();
 	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void NewsClient::write_number(unsigned int value) {
 	conn->write((value >> 24) & 0xFF);
 	conn->write((value >> 16) & 0xFF);
 	conn->write((value >> 8)	 & 0xFF);
 	conn->write(value & 0xFF);
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	shared_ptr<Connection> conn(make_shared<Connection>(argv[1], port));
	if (!conn->isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	NewsClient news_client;
	news_client.set_conn(conn);

	while (true) {
		try {
			cout << "Type a command: ";
			string line;
			getline(cin, line);

			auto space = line.find(' ');
			string first_w	/*cout << "created article with id: " << id<<", title: " << title
	<<", author: " << author << " and text: " << text <<endl;*/list_newsgroups();
			} else if (first_word == "cn") {
				if (space == string::npos) {
					cout << "Must enter a name. Exiting..." << endl;
					exit(1);
				}
				string name = line.substr(space + 1, string::npos);
				news_client.create_newsgroup(name);
			} else if (first_word == "dn") {
				if (space == string::npos) {
					cout << "Must enter an id. Exiting..." << endl;
					exit(1);
				}
				string id_str = line.substr(space + 1, string::npos);
				unsigned int id = stoul(id_str);
				news_client.delete_newsgroup(id);
			}	else if (first_word == "la") {
				if (space == string::npos) {
					cout << "Must enter an id. Exiting..." << endl;
					exit(1);
				}
				string id_str = line.substr(space + 1, string::npos);
				unsigned int id = stoul(id_str);
				news_client.list_articles(id);
			} else if (first_word == "ca") {
				if (space == string::npos) {
					cout << "Must enter more than ca. Exiting..." << endl;
					exit(1);
				}

				string input = line.substr(space + 1, string::npos);
				istringstream iss;
				iss.str(input);

				string id_str;
				iss >> id_str;
				unsigned int id = stoul(id_str);

				string title;
				iss >> title;
				if (title == "") {
					cout << "Must input title. Exiting..." << endl;
					exit(1);
				}

				string author;
				iss >> author;
				if (author == "") {
					cout << "Must input author. Exiting..." << endl;
					exit(1);
				}

				string word;
				string text;
				while (iss >> word) {
					text += word +" ";
				}

				news_client.create_article(id, title, author, text);
			} else {
				cout << "Wrong response" << endl;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting..." << endl;
			exit(1);
		}
	}
}
