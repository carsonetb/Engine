#pragma once
#include <map>
#include <string>
#include "../utils/utils.h"

TVarObj getVarVal(std::string var, std::map<std::string, TVarObj> programVars);
std::string convertVarToString(TVarObj input, std::map<std::string, TVarObj> programVars);
TVarObj getMappableVar(std::string varType, std::string varValue);
TListItem getListItemFromVar(std::string varType, std::string varValue);
TVarObj getPointerVal(TPointer pointer, std::map<std::string, TIterator> iteratorsNameAccess, std::map<std::string, TVarObj> programVars);