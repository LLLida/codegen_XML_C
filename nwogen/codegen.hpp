#pragma once

#include <stdexcept>

#include <rapidxml/rapidxml.hpp>

#include "backend.hpp"
#include "block.hpp"

namespace nwogen {

class CodeGenerator {

  LookupTable blocks;
  std::shared_ptr<Block> outport;

public:

  CodeGenerator(rapidxml::xml_document<>& document);

  void generateCode(Backend& backend);

};

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

}
