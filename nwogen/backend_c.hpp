#pragma once

#include "backend.hpp"

#include <ostream>
#include <sstream>
#include <vector>

namespace nwogen {

class Backend_C : public Backend
{
  std::vector<std::string> variables;
  std::vector<std::pair<std::string, std::string>> inits;
  std::vector<std::tuple<std::string, std::string, bool, int>> ports;
  std::vector<std::pair<std::string, std::string>> steps1;
  std::vector<std::pair<std::string, std::string>> steps2;

public:

  Backend_C();

  void declareVariable(const std::string& var) override;
  void initVariable(const std::string& var, const std::string& expr) override;
  void addPort(const std::string& name, const std::string& var, bool isInput, int portNumber) override;
  void addStep(const std::string& name, const std::string& expr, bool post) override;

  void saveCode(std::ostream& out) const override;

};

}
