#pragma once

#include <memory>
#include <stdexcept>

#include <rapidxml/rapidxml.hpp>

#include "backend.hpp"
#include "block.hpp"

namespace nwogen {

class CodeGenerator {

  std::vector<std::shared_ptr<Block>> blocks;

public:

  CodeGenerator(rapidxml::xml_document<>& document);

  void generateCode(const Backend& backend, std::ostream& out);

};

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

}
