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

BlockOutport::BlockOutport(int64_t SID, const std::string& name)
  : Block(SID, name)
{}

void BlockOutport::write(const Backend& backend) const {
}

BlockSum::BlockSum(int64_t SID, const std::string& name, int64_t left, int64_t right)
  : Block(SID, name), leftSID(left), rightSID(right)
{

}

void BlockSum::write(const Backend& backend) const {
}

int64_t BlockSum::getLeft() const {
  return leftSID;
}

int64_t BlockSum::getRight() const {
  return rightSID;
}

BlockGain::BlockGain(int64_t SID, const std::string& name, int64_t input, double factor)
  : Block(SID, name), input(input), factor(factor)
{}

void BlockGain::write(const Backend& backend) const
{

}

int64_t BlockGain::getInput() const
{
  return input;
}

BlockUnitDelay::BlockUnitDelay(int64_t SID, const std::string& name, int64_t input, double sampleTime)
  : Block(SID, name), input(input), sampleTime(sampleTime)
{}

void BlockUnitDelay::write(const Backend& backend) const {

}

int64_t BlockUnitDelay::getInput() const {
  return input;
}

} // namespace nwogen
