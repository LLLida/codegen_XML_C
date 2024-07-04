#pragma once

#include <memory>
#include <string>
#include <vector>

namespace nwogen {

class Backend
{
public:
  Backend() = default;
  Backend(const Backend&) = delete;
  Backend(Backend&&) = delete;

  virtual ~Backend() = default;

  virtual void declareVariable(const std::string& var, int order = 0) = 0;
  virtual void initVariable(const std::string& var, const std::string& expr) = 0;
  virtual void addPort(const std::string& name, const std::string& var, bool isInput, int portNumber) = 0;
  virtual void addStep(const std::string& name, const std::string& expr, bool post = false) = 0;

  virtual void addStepIdentity(const std::string& name, const std::string& argument, bool post = false) = 0;
  virtual void addStepAddition(const std::string& name, const std::string& left, const std::string& right, bool isLeftNegative, bool isRightNegative, bool post = false) = 0;
  virtual void addStepMultiplication(const std::string& name, const std::string& left, const std::string& right, bool post = false) = 0;

  virtual void addStepDependency(const std::string& dependencyName, const std::vector<std::string>& dependencyInputs,
				 const std::string& dependencyOutput) = 0;

  virtual void saveCode(std::ostream& out) const = 0;

  virtual std::shared_ptr<Backend> copy() const = 0;
};

}
