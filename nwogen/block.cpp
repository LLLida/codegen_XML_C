#include "block.hpp"
#include "diagram.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

std::string removeSpaces(std::string str) {
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  return str;
}

namespace nwogen {

void mark(int64_t SID, const LookupTable& table) {
  auto& [unused, marked] = table.at(SID);
  const_cast<bool&>(marked) = true;
}

Block::Block(int64_t SID, const std::string& name)
  : SID(SID), name(removeSpaces(name))
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

  // give inports have priority, so they get declared first
  backend.declareVariable(getName(), getSID()-100);
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

BlockSum::BlockSum(int64_t SID, const std::string& name, int64_t leftSID, int64_t rightSID, bool isLeftMinus, bool isRightMinus)
  : Block(SID, name), leftSID(leftSID), rightSID(rightSID), isLeftMinus(isLeftMinus), isRightMinus(isRightMinus)
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

  backend.declareVariable(getName(), getSID());
  backend.addStepAddition(getName(), left->getName(), right->getName(), isLeftMinus, isRightMinus);
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

  backend.declareVariable(getName(), getSID());
  backend.addStepMultiplication(getName(), input->getName(), std::to_string(factor));
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

  backend.declareVariable(getName(), getSID());
  backend.initVariable(getName(), "0");
  backend.addStepIdentity(getName(), input->getName(), true);
}

int64_t BlockUnitDelay::getInputSID() const {
  return inputSID;
}

BlockCustom::BlockCustom(int64_t SID, const std::string& name, const std::shared_ptr<Diagram>& diagram, const std::string& filepath, const std::vector<int>& dependencies)
  : Block(SID, name), diagram(diagram), filepath(filepath), dependencies(dependencies)
{}

void BlockCustom::write(Backend& backend, const LookupTable& table) const {
  mark(getSID(), table);

  for (int inputSID: dependencies) {
    auto& [input, marked] = table.at(inputSID);
    if (!marked) {
      input->write(backend, table);
    }
  }

  auto newBackend = backend.copy();
  diagram->emit(*newBackend);
  std::ofstream fout(filepath);
  newBackend->saveCode(fout);

  std::vector<std::string> depNames(dependencies.size());
  std::transform(dependencies.begin(), dependencies.end(), depNames.begin(),
		 [&table](int sid) {
		   return table.at(sid).first->getName();
		 });
  backend.declareVariable(getName(), getSID());
  backend.addStepDependency(filepath, depNames, getName());
}

const std::shared_ptr<Diagram>& BlockCustom::getDiagram() const {
  return diagram;
}

} // namespace nwogen
