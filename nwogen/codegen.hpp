#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <rapidxml/rapidxml.hpp>

#include "backend.hpp"
#include "block.hpp"

namespace nwogen {

class CodeGenerator {

  std::unordered_map<int, std::shared_ptr<Block>> blocks;
  std::shared_ptr<Block> outport;

public:

  CodeGenerator(rapidxml::xml_document<>& document);

  void generateCode(const Backend& backend, std::ostream& out);

};

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

}
