#pragma once

#include <string>
#include <vector>

namespace nwogen {

class Backend
{
public:
  enum class OperationType {
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    SIN,
    COS,
    EXP,
    POW
  };

  virtual ~Backend() = default;

  virtual void declareVariable(const std::string& var) = 0;
  virtual void initVariable(const std::string& var, double value) = 0;
  virtual void addPort(const std::string& name, const std::string& var, bool isInput, int portNumber) = 0;

  virtual void beginAssignment(const std::string& var) = 0;
  virtual void endAssignment(const std::string& var) = 0;
  virtual void addExpression(OperationType op, const std::vector<std::string>& variables) = 0;

  virtual void saveCode(std::ostream& out) const = 0;
};

}
