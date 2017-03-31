/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iterator>
#include "protocol.h"

using namespace std;

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

	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	while (true) {
		try {
			cout << "Type a command: ";
			string line;
			getline(cin, line);
			auto space = line.find(' ');
			string first_word = line.substr(0, space);
			
			if (first_word == "ls") {
				conn.write(Protocol::COM_LIST_NG);
				conn.write(Protocol::COM_END);
			}
			if (first_word == "c") {
				if (space == string::npos) {
					cout << "Must enter a name. Exiting" << endl;
					exit(1);
				}

				//Print functions (delete later)
				/*string name = line.substr(space + 1, string::npos);
				cout << Protocol::COM_CREATE_NG << " " << Protocol::PAR_STRING
					<< " " << name.size() << " ";
				for (char c : name) 
					cout << c;
				cout << " " << Protocol::COM_END;*/
				//End print functions

				conn.write(Protocol::COM_CREATE_NG);
				conn.write(Protocol::PAR_STRING);
				conn.write(name.size());
				for (char c : name) 
					conn.write(c);
				conn.write(Protocol::COM_END);
			}
			if (first_word == "d") {
				if (space == string::npos) {
					cout << "Must enter an id. Exiting" << endl;
					exit(1);
				}
				conn.write(Protocol::COM_DELETE_NG);
				char id = line.substr(space + 1, string::npos);
				conn.write(Protocol::PAR_NUM);
				conn.write(id);
				conn.write(Protocol::PAR_END);
			}
			if (first_word == "la") {
				if (space == string::npos) {
					cout << "Must enter an id. Exiting" << endl;
					exit(1);
				}
				conn.write(Protocol::COM_LIST_ART);
				char id = line.substr(space + 1, string::npos);
				conn.write(Protocol::PAR_NUM);
				conn.write(id);
				conn.write(Protocol::PAR_END);
			}
				
			cout << "Reply is " << conn.read() << endl;
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}
