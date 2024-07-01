#pragma once

#include <memory>
#include <string>

#include "backend.hpp"

namespace nwogen {

class Block {

  int64_t SID;
  std::string name;

protected:

  Block(int64_t SID, const std::string& name);

public:

  int64_t getSID() const;
  const std::string& getName() const;

  virtual ~Block() = default;

  virtual void write(const Backend& backend) const = 0;

};

class BlockInport : public Block {

public:

  BlockInport(int64_t SID, const std::string& name);

  void write(const Backend& backend) const override;

};

class BlockOutport : public Block {

public:

  Outport(int64_t SID, const std::string& name);

  void write(const Backend& backend) const override;

};

class BlockSum : public Block {

  std::shared_ptr<Block> left_argument;
  std::shared_ptr<Block> right_argument;

public:

  BlockSum(int64_t SID, const std::string& name, const std::shared_ptr<Block>& left, const std::shared_ptr<Block>& right);

  void write(const Backend& backend) const override;

};

class BlockGain : public Block {

public:

  BlockGain(int64_t SID, const std::string& name, const std::shared_ptr<Block>& block);

  void write(const Backend& backend) const override;

};

class BlockUnitDelay : public Block {

  std::shared_ptr<Block> parent;

public:

  BlockUnitDelay(int64_t SID, const std::string& name, const std::shared_ptr<Block>& parent);

  void write(const Backend& backend) const override;

};

}
