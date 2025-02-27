#include "diagram.hpp"

#include <cstring>
#include <fstream>
#include <iostream>		// for operator <<(ostream, char*)
#include <sstream>
#include <string>

using namespace std::literals;

namespace nwogen {

static std::unordered_map<int, std::vector<int>> parseEdges(rapidxml::xml_node<>* rootNode);

static std::shared_ptr<Block> parseInport(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges);
static std::shared_ptr<Block> parseOutport(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges);
static std::shared_ptr<Block> parseSum(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges);
static std::shared_ptr<Block> parseGain(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges);
static std::shared_ptr<Block> parseUnitDelay(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges);
static std::shared_ptr<Block> parseCustom(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges);

Diagram::Diagram(rapidxml::xml_document<>& document)
  : blocks(), outport(nullptr)
{
  rapidxml::xml_node<>* systemNode = document.first_node();
  if (strcmp(systemNode->name(), "System") != 0) {
    throw ParseError("Expected root node to be called \"System\"");
  }

  using ParseFunction = std::shared_ptr<Block>(*)(rapidxml::xml_node<>*, const std::unordered_map<int, std::vector<int>>&);
  std::unordered_map<std::string, ParseFunction> parseFunctions;
  parseFunctions["Inport"] = parseInport;
  parseFunctions["Outport"] = parseOutport;
  parseFunctions["Sum"] = parseSum;
  parseFunctions["Gain"] = parseGain;
  parseFunctions["UnitDelay"] = parseUnitDelay;
  parseFunctions["Custom"] = parseCustom;

  std::unordered_map<int, std::vector<int>> edges = parseEdges(systemNode);

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

    auto block = parseFunctions[blockType->value()](node, edges);
    blocks[sidValue] = {block, false};
    if (strcmp(blockType->value(), "Outport") == 0) {
      if (outport) {
	throw ParseError("there should only 1 block with type Outport");
      }
      outport = std::dynamic_pointer_cast<BlockOutport>(block);
    }
  }

  if (!outport) {
    throw ParseError("there must be 1 block with type Outport");
  }
}

void Diagram::emit(Backend& backend) {
  outport->write(backend, blocks);
}

const std::shared_ptr<BlockOutport> Diagram::getOutport() const {
  return outport;
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
	  src = atoi(child->value());
	} else if (strcmp(name->value(), "Dst") == 0) {
	  dst = atoi(child->value());
	}
      }

    }
    if (src == -1) {
      throw ParseError("Expected Line to have Src");
    }
    if (dst != -1) {
      edges[dst].push_back(src);
    }

    for (auto branch = node->first_node("Branch"); branch; branch = branch->next_sibling("Branch")) {

      dst = -1;
      for (auto child = branch->first_node("P"); child; child = child->next_sibling("P")) {
	auto name = child->first_attribute("Name");
	if (name) {
	  if (strcmp(name->value(), "Dst") == 0) {
	    dst = atoi(child->value());
	  }
	}
      }
      if (dst == -1) {
	throw ParseError("Expected Branch to have Dst");
      }
      edges[dst].push_back(src);

    }
  }

  return edges;
}

std::shared_ptr<Block> parseInport(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges)
{
  auto name = blockNode->first_attribute("Name");
  int64_t sidValue = atoll(blockNode->first_attribute("SID")->value());

  auto it = edges.find(sidValue);
  if (it != edges.end()) {
    throw ParseError("there should not be any lines pointing to Inport");
  }

  // ignoring port field...
  // auto port = blockNode->first_node("Port");
  // if (!port) {
  //   throw ParseError("block Inport should have child node named Port");
  // }
  int portNumber = 1;
  for (auto p = blockNode->first_node("P"); p; p = p->next_sibling("P")) {
    auto name = p->first_attribute("Name");
    if (!name) {
      throw ParseError("<P> has no Name");
    }
    if (strcmp(name->value(), "Port") == 0) {
      portNumber = atoi(p->value());
    }
  }

  // std::cout << "Inport: (" << sidValue << ") " << name->value() << " portNumber=" << portNumber << "\n";
  return std::make_shared<BlockInport>(sidValue, name->value(), portNumber);
}

std::shared_ptr<Block> parseOutport(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges)
{
  auto name = blockNode->first_attribute("Name");
  int64_t sidValue = atoll(blockNode->first_attribute("SID")->value());

  auto it = edges.find(sidValue);
  if (it == edges.end()) {
    throw ParseError("SID "+std::to_string(sidValue)+" has not been defined");
  }
  auto& srcVector = it->second;
  if (srcVector.size() != 1) {
    throw ParseError("expected Gain to have 1 input");
  }
  auto input = srcVector[0];

  // std::cout << "Outport: (" << sidValue << ") " << name->value() << " input=" << input << "\n";
  return std::make_shared<BlockOutport>(sidValue, name->value(), input);
}

std::shared_ptr<Block> parseSum(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges)
{
  auto name = blockNode->first_attribute("Name");
  int64_t sidValue = atoll(blockNode->first_attribute("SID")->value());

  auto it = edges.find(sidValue);
  if (it == edges.end()) {
    throw ParseError("SID "+std::to_string(sidValue)+" has not been defined");
  }
  auto& srcVector = it->second;
  if (srcVector.size() != 2) {
    throw ParseError("expected Sum to have 2 inputs");
  }
  auto firstInput = srcVector[0];
  auto secondInput = srcVector[1];

  bool isLeftMinus = false, isRightMinus = false;
  for (auto p = blockNode->first_node("P"); p; p = p->next_sibling("P")) {
    auto name = p->first_attribute("Name");
    if (!name) {
      throw ParseError("<P> has no Name");
    }
    if (strcmp(name->value(), "Inputs") == 0) {
      isLeftMinus = p->value()[0] == '-';
      isRightMinus = p->value()[1] == '-';
    }
  }

  // std::cout << "Sum: (" << sidValue << ") " << name->value() << " left=" << firstInput << " right=" << secondInput << "\n";
  return std::make_shared<BlockSum>(sidValue, name->value(), firstInput, secondInput, isLeftMinus, isRightMinus);
}

std::shared_ptr<Block> parseGain(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges)
{
  auto name = blockNode->first_attribute("Name");
  int64_t sidValue = atoll(blockNode->first_attribute("SID")->value());

  auto it = edges.find(sidValue);
  if (it == edges.end()) {
    throw ParseError("SID "+std::to_string(sidValue)+" has not been defined");
  }
  auto& srcVector = it->second;
  if (srcVector.size() != 1) {
    throw ParseError("expected Gain to have 1 input");
  }
  auto input = srcVector[0];

  double gain = -1;
  for (auto p = blockNode->first_node("P"); p; p = p->next_sibling("P")) {
    auto name = p->first_attribute("Name");
    if (!name) {
      throw ParseError("<P> has no Name");
    }
    if (strcmp(name->value(), "Gain") == 0) {
      gain = strtod(p->value(), NULL);
    }
  }
  if (gain == -1) {
    throw ParseError("block Gain has no Gain parameter specified");
  }

  // std::cout << "Gain: (" << sidValue << ") " << name->value() << " gain=" << gain << " input=" << input << "\n";
  return std::make_shared<BlockGain>(sidValue, name->value(), input, gain);
}

std::shared_ptr<Block> parseUnitDelay(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges)
{
  auto name = blockNode->first_attribute("Name");
  int64_t sidValue = atoll(blockNode->first_attribute("SID")->value());

  auto it = edges.find(sidValue);
  if (it == edges.end()) {
    throw ParseError("SID "+std::to_string(sidValue)+" has not been defined");
  }
  auto& srcVector = it->second;
  if (srcVector.size() != 1) {
    throw ParseError("expected UnitDelay to have 1 input");
  }
  auto input = srcVector[0];

  double sampleTime = -9999;
  for (auto p = blockNode->first_node("P"); p; p = p->next_sibling("P")) {
    auto name = p->first_attribute("Name");
    if (!name) {
      throw ParseError("<P> has no Name");
    }
    if (strcmp(name->value(), "SampleTime") == 0) {
      sampleTime = strtod(p->value(), NULL);
    }
  }
  if (sampleTime == -9999) {
    throw ParseError("block UnitDelay has no SampleTime parameter specified");
  }

  // std::cout << "UnitDelay: (" << sidValue << ") " << name->value() << " sampleTime=" << sampleTime << " input=" << input << "\n";
  return std::make_shared<BlockUnitDelay>(sidValue, name->value(), input, sampleTime);
}

std::shared_ptr<Block> parseCustom(rapidxml::xml_node<>* blockNode, const std::unordered_map<int, std::vector<int>>& edges)
{
  std::string name = blockNode->first_attribute("Name")->value();
  int64_t sidValue = atoll(blockNode->first_attribute("SID")->value());

  auto it = edges.find(sidValue);
  if (it == edges.end()) {
    throw ParseError("SID "+std::to_string(sidValue)+" has not been defined");
  }
  auto& srcVector = it->second;

  std::string filepath;
  std::string outpath;
  for (auto p = blockNode->first_node("P"); p; p = p->next_sibling("P")) {
    auto name = p->first_attribute("Name");
    if (!name) {
      throw ParseError("<P> has no Name");
    }
    if (strcmp(name->value(), "Filepath") == 0) {
      filepath = p->value();
    } else if (strcmp(name->value(), "OutputFile") == 0) {
      outpath = p->value();
    }
  }

  std::ifstream t(filepath);
  std::stringstream buffer;
  buffer << t.rdbuf();
  // std::cout << buffer.str() << "\n";
  rapidxml::xml_document<> document;
  document.parse<0>((char*)buffer.str().c_str());
  auto diagram = std::make_shared<Diagram>(document);

  // std::cout << "Custom: (" << sidValue << ") " << name << " outpath=" << outpath << " inputpath=" << filepath << "\n";
  return std::make_shared<BlockCustom>(sidValue, name, diagram, outpath, srcVector);
}

}
