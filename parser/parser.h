// T Language Interpretter Header
// ----------------------------------------------------

// T parser, central handler library for interpretting.

#pragma once
#include <string>

std::string readFile(std::string fileName);
int parseRun(std::string fileName);