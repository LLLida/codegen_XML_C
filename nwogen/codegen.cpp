#include "codegen.hpp"

#include <cstring>
#include <iostream>		// for operator <<(ostream, char*)
#include <string>
#include <unordered_map>

namespace nwogen {

static void parseObject(rapidxml::xml_node<>* node);

/*
std::exception <exception> interface (debatable if you should catch this)
    std::bad_alloc <new> failure to allocate storage
	std::bad_array_new_length <new> invalid array length
    std::bad_cast <typeinfo> execution of an invalid dynamic-cast
    std::bad_exception <exception> signifies an incorrect exception was thrown
    std::bad_function_call <functional> thrown by "null" std::function
    std::bad_typeid <typeinfo> using typeinfo on a null pointer
    std::bad_weak_ptr <memory> constructing a shared_ptr from a bad weak_ptr
    std::logic_error <stdexcept> errors detectable before the program executes
	std::domain_error <stdexcept> parameter outside the valid range
	std::future_error <future> violated a std::promise/std::future condition
	std::invalid_argument <stdexcept> invalid argument
	std::length_error <stdexcept> length exceeds its maximum allowable size
	std::out_of_range <stdexcept> argument value not in its expected range
    std::runtime_error <stdexcept> errors detectable when the program executes
	std::overflow_error <stdexcept> arithmetic overflow error.
	std::underflow_error <stdexcept> arithmetic underflow error.
	std::range_error <stdexcept> range errors in internal computations
	std::regex_error <regex> errors from the regular expression library.
	std::system_error <system_error> from operating system or other C API
	    std::ios_base::failure <ios> Input or output error
 */

CodeGenerator::CodeGenerator(rapidxml::xml_document<>& document)
{
  rapidxml::xml_node<>* systemNode = document.first_node();
  if (strcmp(systemNode->name(), "System") != 0) {
    throw ParseError("Expected root node to be called \"System\"");
  }

  for (rapidxml::xml_node<>* node = systemNode->first_node(); node; node = node->next_sibling()) {

    if (strcmp(node->name(), "Block") == 0) {
      auto block_type = node->first_attribute("BlockType");
      if (block_type == nullptr) {
	throw ParseError("Expected block node to have attribute \"BlockType\"");
      }
      auto name = node->first_attribute("Name");
      if (name == nullptr) {
	throw ParseError("Expected block node to have attribute \"Name\"");
      }
      auto sid = node->first_attribute("SID");
      if (sid == nullptr) {
	throw ParseError("Expected block node to have attribute \"sid\"");
      }
      std::cout << "Node: " << name->value() << " " << block_type->value() << " " << sid->value() << "\n";

      parseObject(node);

    } else if (strcmp(node->name(), "Line") == 0) {

    } else {
      throw ParseError("Expected node to be called \"Block\" or \"Line\"");
    }

    if (node->value())
      std::cout << "Node Value: " << node->value() << std::endl;
  }

}

void CodeGenerator::generateCode(const Backend& backend, std::ostream& out) {
  // NOTE: generateCode assumes that blocks are already sorted in topological order
}


void parseObject(rapidxml::xml_node<>* node)
{
  for (auto child = node->first_node(); child; child = child->next_sibling()) {
    if (strcmp(child->name(), "P") == 0) {
      auto attribute = child->first_attribute("Name");
      if (attribute == nullptr) {
	throw ParseError("Expected child node of block node to have attribute \"Name\"");
      }
      std::cout << "  Field: " << attribute->value() << "=\"" << child->value() << "\"\n";
    } else if (strcmp(child->name(), "Port") == 0) {
      parseObject(child);
    } else {
      std::cout << child->name() << "--\n";
      throw ParseError("Expected child node of block node to have name \"P\" or \"Port\"");
    }
  }
}

}
