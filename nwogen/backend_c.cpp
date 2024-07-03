#include "backend_c.hpp"

#include <algorithm>
#include <fstream>

namespace nwogen {

Backend_C::Backend_C()
{}

void Backend_C::declareVariable(const std::string& var) {
  variables.push_back(var);
}

void Backend_C::initVariable(const std::string& var, const std::string& expr) {
  inits.push_back({var, expr});
}

void Backend_C::addPort(const std::string& name, const std::string& var, bool isInput, int portNumber) {
  ports.push_back({name, var, isInput, portNumber});
}

void Backend_C::addStep(const std::string& name, const std::string& expr, bool post) {
  if (post) {
    steps2.push_back({name, expr});
  } else {
    steps1.push_back({name, expr});
  }
}

void Backend_C::saveCode(std::ostream& out) const
{
  out << "#include \"nwocg_run.h\"\n";
  out << "#include <math.h>\n";

  out << "static struct {\n";
  for (auto& var: variables) {
    out << "\tdouble " << var << ";\n";
  }
  out << "} nwocg;\n";

  out << "void nwocg_generated_init()\n";
  out << "{\n";
  for (auto& init: inits) {
    out << "\tnwocg." << init.first << " = " << init.second << ";\n";
  }
  out << "}\n";

  out << "void nwocg_generated_step()\n";
  out << "{\n";
  {
    for (auto& step: steps1) {
      out << "\tnwocg." << step.first << " = " << step.second << ";\n";
    }
    out << "\n";
    for (auto& step: steps2) {
      out << "\tnwocg." << step.first << " = " << step.second << ";\n";
    }
  }
  out << "}\n";

  out << "static const nwocg_ExtPort ext_ports[] =\n";
  out << "{\n";
  {
    auto tempPorts = ports;
    std::sort(tempPorts.begin(), tempPorts.end(), [](auto& left, auto& right) {
      return std::get<3>(left) > std::get<3>(right);
    });
    for (auto& port: tempPorts) {
      out << "\t{ \"" << std::get<0>(port) << "\", &nwocg." << std::get<1>(port) << ", " << int(std::get<2>(port)) << " },\n";
    }
  }
  out << "\t{ 0, 0, 0 },\n}\n";

  out << "const nwocg_ExtPort * const nwocg_generated_ext_ports = ext_ports;\n const size_t nwocg_generated_ext_ports_size = sizeof(ext_ports);\n";
}

}
