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
  std::unordered_map<int, std::vector<int>> edges;



  for (rapidxml::xml_node<>* node = systemNode->first_node(); node; node = node->next_sibling()) {

    if (strcmp(node->name(), "Block") == 0) {
      auto blockType = node->first_attribute("BlockType");
      if (blockType == nullptr) {
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
      int sidValue = atoi(sid->value());

      if (sid_to_node.find(sidValue) != sid_to_node.end()) {
	throw ParseError("Found two blocks with the same SID");
      }

      sid_to_node[sidValue] = node;

    } else if (strcmp(node->name(), "Line") == 0) {

      int src = -1, dst = -1;
      for (auto child = node->first_node("P"); child; child = child->next_sibling("P")) {

	auto name = child->first_attribute("Name");
	if (name) {
	  if (strcmp(name->value(), "Src") == 0) {
	    std::cout << "\t" << child->value() << " " << atoi(child->value()) << "\n";
	    src = atoi(child->value());
	  } else if (strcmp(name->value(), "Dst") == 0) {
	    std::cout << "\t" << child->value() << " " << atoi(child->value()) << "\n";
	    dst = atoi(child->value());
	  }
	}

      }
      if (dst != -1) {
	edges[dst].push_back(src);
      }

      for (auto branch = node->first_node("Branch"); branch; branch = branch->next_sibling("Branch")) {

	for (auto child = branch->first_node("P"); child; child = child->next_sibling("P")) {
	  auto name = child->first_attribute("Name");
	  if (name) {
	    if (strcmp(name->value(), "Dst") == 0) {
	      dst = atoi(child->value());
	    }
	  }
	}
	edges[dst].push_back(src);

      }

    } else {
      throw ParseError("Expected node to be called \"Block\" or \"Line\"");
    }

  }

  for (auto& it: edges) {
    std::cout << "Dst: " << it.first << " { ";
    for (int a: it.second) {
      std::cout << a << ", ";
    }
    std::cout << "}\n";
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
