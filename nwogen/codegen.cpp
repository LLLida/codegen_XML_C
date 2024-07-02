#include "codegen.hpp"

#include <cstring>
#include <iostream>		// for operator <<(ostream, char*)
#include <string>
#include <unordered_map>

using namespace std::literals;

namespace nwogen {

static std::unordered_map<int, std::vector<int>> parseEdges(rapidxml::xml_node<>* rootNode);

static std::shared_ptr<Block> parseInport(rapidxml::xml_node<>* blockNode);
static std::shared_ptr<Block> parseOutport(rapidxml::xml_node<>* blockNode);
static std::shared_ptr<Block> parseSum(rapidxml::xml_node<>* blockNode);
static std::shared_ptr<Block> parseGain(rapidxml::xml_node<>* blockNode);
static std::shared_ptr<Block> parseUnitDelay(rapidxml::xml_node<>* blockNode);

CodeGenerator::CodeGenerator(rapidxml::xml_document<>& document)
{
  rapidxml::xml_node<>* systemNode = document.first_node();
  if (strcmp(systemNode->name(), "System") != 0) {
    throw ParseError("Expected root node to be called \"System\"");
  }

  using ParseFunction = std::shared_ptr<Block>(*)(rapidxml::xml_node<>*);
  std::unordered_map<std::string, ParseFunction> parseFunctions;
  parseFunctions["Inport"] = parseInport;
  parseFunctions["Outport"] = parseOutport;
  parseFunctions["Sum"] = parseSum;
  parseFunctions["Gain"] = parseGain;
  parseFunctions["UnitDelay"] = parseUnitDelay;

  std::unordered_map<int, std::vector<int>> edges = parseEdges(systemNode);
  std::vector<int> sid_order;
  std::unordered_map<int, std::shared_ptr<Block>> blocks;

  for (rapidxml::xml_node<>* node = systemNode->first_node("Block"); node; node = node->next_sibling("Block")) {
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

    if (blocks.find(sidValue) != blocks.end()) {
      throw ParseError("Found two blocks with the same SID");
    }

    if (parseFunctions.find(blockType->value()) == parseFunctions.end()) {
      throw ParseError("Undefined block type: "s + blockType->value());
    }

    blocks[sidValue] = parseFunctions[blockType->value()](node);

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

std::unordered_map<int, std::vector<int>> parseEdges(rapidxml::xml_node<>* rootNode)
{
  std::unordered_map<int, std::vector<int>> edges;

  for (rapidxml::xml_node<>* node = rootNode->first_node("Line"); node; node = node->next_sibling("Line")) {
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
  }

  return edges;
}

std::shared_ptr<Block> parseInport(rapidxml::xml_node<>* blockNode)
{
  return nullptr;
}

std::shared_ptr<Block> parseOutport(rapidxml::xml_node<>* blockNode)
{
  return nullptr;
}

std::shared_ptr<Block> parseSum(rapidxml::xml_node<>* blockNode)
{
  return nullptr;
}

std::shared_ptr<Block> parseGain(rapidxml::xml_node<>* blockNode)
{
  return nullptr;
}

std::shared_ptr<Block> parseUnitDelay(rapidxml::xml_node<>* blockNode)
{
  return nullptr;
}

}
