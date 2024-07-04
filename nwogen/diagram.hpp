#pragma once

#include <stdexcept>

#include <rapidxml/rapidxml.hpp>

#include "backend.hpp"
#include "block.hpp"

namespace nwogen {

class Diagram {

  LookupTable blocks;
  std::shared_ptr<BlockOutport> outport;

public:

  Diagram(rapidxml::xml_document<>& document);

  void emit(Backend& backend);

  const std::shared_ptr<BlockOutport> getOutport() const;

};

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

}
