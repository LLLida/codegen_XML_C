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

BlockInport::BlockInport(int64_t SID, const std::string& name)
  : Block(SID, name)
{}

void BlockInport::write(const Backend& backend) const {

}

BlockOutport::BlockOutport(int64_t SID, const std::string& name)
  : Block(SID, name)
{}

void BlockOutport::write(const Backend& backend) const {
}

BlockSum::BlockSum(int64_t SID, const std::string& name, const std::shared_ptr<Block>& left, const std::shared_ptr<Block>& right)
  : Block(SID, name), left_argument(left), right_argument(right)
{

}

void BlockSum::write(const Backend& backend) const
{
}


BlockGain::BlockGain(int64_t SID, const std::string& name, const std::shared_ptr<Block>& block)
  : Block(SID, name), block(block)
{}

void BlockGain::write(const Backend& backend) const
{

}

BlockUnitDelay::BlockUnitDelay(int64_t SID, const std::string& name, const std::shared_ptr<Block>& parent)
  : BlockUnitDelay(SID, name), parent(parent)
{}

void BlockUnitDelay::write(const Backend& backend) const {

}

} // namespace nwogen
