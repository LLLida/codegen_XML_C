#include "nwogen/diagram.hpp"
#include "nwogen/backend_c.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv)
{
  if (argc != 2 && argc != 3) {
    std::cout << "Usage: " << argv[0] << " FILEPATH [OUTPATH]\n";
    return 1;
  }
  const char* filepath = argv[1];
  const char* outpath = NULL;
  if (argc > 2) {
    outpath = argv[2];
  }
  std::ifstream t(filepath);
  std::stringstream buffer;
  buffer << t.rdbuf();

  rapidxml::xml_document<> document;
  document.parse<0>((char*)buffer.str().c_str());

  try {
    nwogen::Diagram diagram(document);
    auto backend = std::make_unique<nwogen::Backend_C>();

    diagram.emit(*backend);

    if (outpath) {
      std::ofstream fout(outpath);
      backend->saveCode(fout);
      std::cout << "Saved C source file to \"" << outpath << "\"\n";
    } else {
      backend->saveCode(std::cout);
    }
  } catch (nwogen::ParseError& err) {
    std::cout << "Caught parse error: " << err.what() << "\n";
  }

  return 0;
}
