#include "backend_c.hpp"

#include <fstream>

namespace nwogen {

void Backend_C::declareVariable(const std::string& var)
{

}

void Backend_C::initVariable(const std::string& var, double value)
{

}

void Backend_C::addPort(const std::string& name, const std::string& var, bool isInput)
{

}

void Backend_C::beginAssignment(const std::string& var)
{

}

void Backend_C::endAssignment(const std::string& var)
{

}

void Backend_C::addExpression(OperationType op, const std::vector<std::string>& variables)
{

}

std::string Backend_C::generateCode() const
{
  return "";
}

}
