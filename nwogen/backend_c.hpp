#pragma once

#include "backend.hpp"

#include <sstream>

namespace nwogen {

class Backend_C : public Backend
{
  std::stringstream declarations;
  std::stringstream inits;
  std::stringstream ports;
  std::stringstream assignments;

public:

  void declareVariable(const std::string& var) override;
  void initVariable(const std::string& var, double value) override;
  void addPort(const std::string& name, const std::string& var, bool isInput) override;
  void beginAssignment(const std::string& var) override;
  void endAssignment(const std::string& var) override;
  void addExpression(OperationType op, const std::vector<std::string>& variables) override;

  std::string generateCode() const override;

};

}
