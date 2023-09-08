// Graphics Handler Header
// --------------------------------------------------

// Open, close, draw on, and interact with graphics
// windows.

#pragma once
#include <string>
#include <map>
#include "../utils/utils.h"

TVarObj handleGraphicsFunctionCall(std::string callLine, std::map<std::string, TVarObj> programVars, 
                                   std::map<std::string, TIterator> iteratorsNameAccess);