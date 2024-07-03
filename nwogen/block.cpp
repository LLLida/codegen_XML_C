#include "block.hpp"

#include <iostream>

namespace nwogen {

void mark(int64_t SID, const LookupTable& table) {
  auto& [unused, marked] = table.at(SID);
  const_cast<bool&>(marked) = true;
}

Block::Block(int64_t SID, const std::string& name)
  : SID(SID), name(name)
{}

int64_t Block::getSID() const {
  return SID;
}

const std::string& Block::getName() const {
  return name;
}

BlockInport::BlockInport(int64_t SID, const std::string& name, int portNumber)
  : Block(SID, name), portNumber(portNumber)
{}

void BlockInport::write(Backend& backend, const LookupTable& table) const {
  mark(getSID(), table);

  backend.declareVariable(getName());
  backend.addPort(getName(), getName(), true, portNumber);
}

BlockOutport::BlockOutport(int64_t SID, const std::string& name, int64_t inputSID)
  : Block(SID, name), inputSID(inputSID)
{}

void BlockOutport::write(Backend& backend, const LookupTable& table) const {
  mark(getSID(), table);

  auto& [input, marked] = table.at(inputSID);
  if (!marked) {
    input->write(backend, table);
  }

  backend.addPort(getName(), input->getName(), false, 999);
}

int64_t BlockOutport::getInputSID() const {
  return inputSID;
}

BlockSum::BlockSum(int64_t SID, const std::string& name, int64_t leftSID, int64_t rightSID)
  : Block(SID, name), leftSID(leftSID), rightSID(rightSID)
{

}

void BlockSum::write(Backend& backend, const LookupTable& table) const {
  mark(getSID(), table);

  auto& [left, leftMarked] = table.at(leftSID);
  if (!leftMarked) {
    left->write(backend, table);
  }
  auto& [right, rightMarked] = table.at(rightSID);
  if (!rightMarked) {
    right->write(backend, table);
  }

  backend.declareVariable(getName());
}

int64_t BlockSum::getLeftSID() const {
  return leftSID;
}

int64_t BlockSum::getRightSID() const {
  return rightSID;
}

BlockGain::BlockGain(int64_t SID, const std::string& name, int64_t inputSID, double factor)
  : Block(SID, name), inputSID(inputSID), factor(factor)
{}

void BlockGain::write(Backend& backend, const LookupTable& table) const {
  mark(getSID(), table);

  auto& [input, marked] = table.at(inputSID);
  if (!marked) {
    input->write(backend, table);
  }

  backend.declareVariable(getName());
}

int64_t BlockGain::getInputSID() const
{
  return inputSID;
}

BlockUnitDelay::BlockUnitDelay(int64_t SID, const std::string& name, int64_t inputSID, double sampleTime)
  : Block(SID, name), inputSID(inputSID), sampleTime(sampleTime)
{}

void BlockUnitDelay::write(Backend& backend, const LookupTable& table) const {
  mark(getSID(), table);

  auto& [input, marked] = table.at(inputSID);
  if (!marked) {
    input->write(backend, table);
  }

  // backend.declareVariable(getName());
}

int64_t BlockUnitDelay::getInputSID() const {
  return inputSID;
}

} // namespace nwogen
