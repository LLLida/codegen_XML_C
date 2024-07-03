#include "nwogen/codegen.hpp"
#include "nwogen/backend_c.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " FILEPATH\n";
    return 1;
  }
  const char* filepath = argv[1];
  std::ifstream t(filepath);
  std::stringstream buffer;
  buffer << t.rdbuf();

  rapidxml::xml_document<> document;
  document.parse<0>((char*)buffer.str().c_str());

  try {
    nwogen::CodeGenerator codegen(document);
    auto backend = std::make_shared<nwogen::Backend_C>(std::cout);

    codegen.generateCode(*backend);
  } catch (nwogen::ParseError& err) {
    std::cout << "Caught parse error: " << err.what() << "\n";
  }

  return 0;
}
