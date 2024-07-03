#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "backend.hpp"

namespace nwogen {

class Block;
using LookupTable = std::unordered_map<int, std::pair<std::shared_ptr<Block>, bool>>;

class Block {

  int64_t SID;
  std::string name;

protected:

  Block(int64_t SID, const std::string& name);

public:

  int64_t getSID() const;
  const std::string& getName() const;

  virtual ~Block() = default;

  virtual void write(Backend& backend, const LookupTable& table) const = 0;

};

class BlockInport : public Block {

  int portNumber;

public:

  BlockInport(int64_t SID, const std::string& name, int portNumber);

  void write(Backend& backend, const LookupTable& table) const override;

};

class BlockOutport : public Block {

  int64_t inputSID;

public:

  BlockOutport(int64_t SID, const std::string& name, int64_t inputSID);

  void write(Backend& backend, const LookupTable& table) const override;

  int64_t getInputSID() const;

};

class BlockSum : public Block {

  int64_t leftSID;
  int64_t rightSID;

public:

  BlockSum(int64_t SID, const std::string& name, int64_t leftSID, int64_t rightSID);

  void write(Backend& backend, const LookupTable& table) const override;

  int64_t getLeftSID() const;
  int64_t getRightSID() const;

};

class BlockGain : public Block {

  double factor;
  int64_t inputSID;

public:

  BlockGain(int64_t SID, const std::string& name, int64_t inputSID, double factor);

  void write(Backend& backend, const LookupTable& table) const override;

  int64_t getInputSID() const;

};

class BlockUnitDelay : public Block {

  int64_t inputSID;
  double sampleTime;

public:

  BlockUnitDelay(int64_t SID, const std::string& name, int64_t inputSID, double sampleTime);

  void write(Backend& backend, const LookupTable& table) const override;

  int64_t getInputSID() const;
};

}
