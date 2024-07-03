#pragma once

#include "backend.hpp"

#include <ostream>
#include <sstream>
#include <vector>

namespace nwogen {

class Backend_C : public Backend
{
  std::vector<std::string> variables;
  std::stringstream inits;
  std::vector<std::tuple<std::string, std::string, bool, int>> ports;
  std::stringstream steps;
  std::stringstream updates;

public:

  Backend_C();

  void declareVariable(const std::string& var) override;
  void initVariable(const std::string& var, double value) override;
  void addPort(const std::string& name, const std::string& var, bool isInput, int portNumber) override;
  void beginAssignment(const std::string& var) override;
  void endAssignment(const std::string& var) override;
  void addExpression(OperationType op, const std::vector<std::string>& variables) override;

  void saveCode(std::ostream& out) const override;

};

}
