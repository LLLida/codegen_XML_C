#pragma once

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

  int portNumber;

public:

  BlockInport(int64_t SID, const std::string& name, int portNumber);

  void write(const Backend& backend) const override;

};

class BlockOutport : public Block {

  int64_t inputSID;

public:

  BlockOutport(int64_t SID, const std::string& name, int64_t inputSID);

  void write(const Backend& backend) const override;

  int64_t getInputSID() const;

};

class BlockSum : public Block {

  int64_t leftSID;
  int64_t rightSID;

public:

  BlockSum(int64_t SID, const std::string& name, int64_t leftSID, int64_t rightSID);

  void write(const Backend& backend) const override;

  int64_t getLeftSID() const;
  int64_t getRightSID() const;

};

class BlockGain : public Block {

  double factor;
  int64_t inputSID;

public:

  BlockGain(int64_t SID, const std::string& name, int64_t inputSID, double factor);

  void write(const Backend& backend) const override;

  int64_t getInputSID() const;

};

class BlockUnitDelay : public Block {

  int64_t inputSID;
  double sampleTime;

public:

  BlockUnitDelay(int64_t SID, const std::string& name, int64_t inputSID, double sampleTime);

  void write(const Backend& backend) const override;

  int64_t getInputSID() const;
};

}
