#include "block.hpp"

namespace nwogen {

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

void BlockInport::write(const Backend& backend) const {

}

BlockOutport::BlockOutport(int64_t SID, const std::string& name, int64_t inputSID)
  : Block(SID, name), inputSID(inputSID)
{}

void BlockOutport::write(const Backend& backend) const {
}

int64_t BlockOutport::getInputSID() const {
  return inputSID;
}

BlockSum::BlockSum(int64_t SID, const std::string& name, int64_t leftSID, int64_t rightSID)
  : Block(SID, name), leftSID(leftSID), rightSID(rightSID)
{

}

void BlockSum::write(const Backend& backend) const {
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

void BlockGain::write(const Backend& backend) const
{

}

int64_t BlockGain::getInputSID() const
{
  return inputSID;
}

BlockUnitDelay::BlockUnitDelay(int64_t SID, const std::string& name, int64_t inputSID, double sampleTime)
  : Block(SID, name), inputSID(inputSID), sampleTime(sampleTime)
{}

void BlockUnitDelay::write(const Backend& backend) const {

}

int64_t BlockUnitDelay::getInputSID() const {
  return inputSID;
}

} // namespace nwogen
