#include "codegen.hpp"

#include <cstring>
#include <iostream>		// for operator <<(ostream, char*)
#include <string>
#include <unordered_map>

namespace nwogen {

static void parseObject(rapidxml::xml_node<>* node);

CodeGenerator::CodeGenerator(rapidxml::xml_document<>& document)
{
  rapidxml::xml_node<>* systemNode = document.first_node();
  if (strcmp(systemNode->name(), "System") != 0) {
    throw ParseError("Expected root node to be called \"System\"");
  }

  std::unordered_map<int, rapidxml::xml_node<>*> sid_to_node;
  std::unordered_map<int, std::vector<int>> connections;

  for (rapidxml::xml_node<>* node = systemNode->first_node(); node; node = node->next_sibling()) {

    if (strcmp(node->name(), "Block") == 0) {
      auto block_type = node->first_attribute("BlockType");
      if (block_type == nullptr) {
	throw ParseError("Expected block node to have attribute \"BlockType\"");
      }
      auto name = node->first_attribute("Name");
      if (name == nullptr) {
	throw ParseError("Expected block node to have attribute \"Name\"");
      }
      auto sid = node->first_attribute("SID");
      if (sid == nullptr) {
	throw ParseError("Expected block node to have attribute \"sid\"");
      }
      std::cout << "Node: " << name->value() << " " << block_type->value() << " " << sid->value() << "\n";

      parseObject(node);

    } else if (strcmp(node->name(), "Line") == 0) {
      std::cout << "Line:\n";
      parseObject(node);
    } else {
      throw ParseError("Expected node to be called \"Block\" or \"Line\"");
    }

  }

}

void CodeGenerator::generateCode(const Backend& backend, std::ostream& out) {
  // NOTE: generateCode assumes that blocks are already sorted in topological order
}


void parseObject(rapidxml::xml_node<>* node)
{
  for (auto child = node->first_node(); child; child = child->next_sibling()) {
    if (strcmp(child->name(), "P") == 0) {
      auto attribute = child->first_attribute("Name");
      if (attribute == nullptr) {
	throw ParseError("Expected child node of block node to have attribute \"Name\"");
      }
      std::cout << "  Field: " << attribute->value() << "=\"" << child->value() << "\"\n";
    } else if (strcmp(child->name(), "Port") == 0) {
      parseObject(child);
    } else if (strcmp(child->name(), "Branch") == 0) {
      parseObject(child);
    } else {
      std::cout << child->name() << "--\n";
      throw ParseError("Expected child node of block node to have name \"P\" or \"Port\"");
    }
  }
}

}
