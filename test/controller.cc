#include "controller.h"

Controller::Controller() {

}

void Controller::set_conn(std::shared_ptr<Connection> conn) {
	this->conn = conn;
}

unsigned int Controller::read_number() {
  unsigned char byte1 = conn->read();
 	unsigned char byte2 = conn->read();
 	unsigned char byte3 = conn->read();
 	unsigned char byte4 = conn->read();
 	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void Controller::write_number(unsigned int value) {
  conn->write((value >> 24) & 0xFF);
 	conn->write((value >> 16) & 0xFF);
 	conn->write((value >> 8)	 & 0xFF);
 	conn->write(value & 0xFF);
}

void Controller::write_string(std::string msg) {
  for (char c : msg)
	{
		conn->write(c);
	}
}
