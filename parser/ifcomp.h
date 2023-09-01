// If Statement (kwd: if) Computation Header
// ------------------------------------------------

// Library for handling and computing if statements

#pragma once
#include <vector>
#include <string>
#include <map>
#include "../utils/utils.h"

std::vector<std::vector<std::string>> segmentateIf(std::string statement);
bool checkSegmentatedIf(std::vector<std::vector<std::string>> toCheck, std::vector<std::string> compareOperators, 
                        std::map<std::string, TVarObj> programVars, std::map<std::string, TIterator> iteratorsNameAccess, 
                        int i, int linesImported);
bool isCheckTrue(std::string check, std::vector<std::string> compareOperators, std::map<std::string, TVarObj> programVars, 
                 std::map<std::string, TIterator> iteratorsNameAccess, int i, int linesImported);
