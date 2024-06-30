#include "rapidxml/rapidxml.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " FILEPATH\n";
    return 1;
  }
  const char* filepath = argv[1];
  std::ifstream t(filepath);
  std::stringstream buffer;
  buffer << t.rdbuf();

  rapidxml::xml_document<> document;
  document.parse<0>((char*)buffer.str().c_str());

  // std::cout << "Name of my first node is: " << document.first_node()->name() << "\n";
  // rapidxml::xml_node<> *node = document.first_node("System");
  // std::cout << "Node foobar has value " << node->value() << "\n";
  // for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
  //   std::cout << "Node foobar has attribute " << attr->name() << " ";
  //   std::cout << "with value " << attr->value() << "\n";
  // }

  rapidxml::xml_node<>* rootNode = document.first_node()->first_node();

  for (rapidxml::xml_node<>* node = rootNode; node; node = node->next_sibling()) {
    std::cout << "Node Name: " << node->name() << std::endl;

    if (node->value())
      std::cout << "Node Value: " << node->value() << std::endl;

    for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
      std::cout << "  Attribute Name: " << attr->name() << ", Attribute Value: " << attr->value() << std::endl;
    }
  }

  return 0;
}
