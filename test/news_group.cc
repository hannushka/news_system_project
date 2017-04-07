#include "news_group.h"

using namespace std;

NewsGroup::NewsGroup(int id, std::string name) : id(id), name(name) {

}

string NewsGroup::get_name() {
  return name;
}
