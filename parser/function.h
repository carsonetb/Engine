// Function Computation Header
// --------------------------------------------------

// Library for parsing functions.

#include <vector>
#include <string>
#include <tuple>
#include <map>

std::vector<std::tuple<std::string, std::string>> unpackArgumentString(std::string arguments);
std::vector<std::tuple<bool, std::string>> unpackCallArgumentString(std::string arguments);
TFunction unpackFunctionLine(std::string line, int currentLine);
TFunctionCall unpackFunctionCallLine(std::string line);
void callFunction(TFunctionCall toCall, std::map<int, std::tuple<std::string, int, bool, std::string>> &statementInfo,
                  std::map<std::string, TVarObj> &programVars, int &currentLine, int &nestedStatements, std::map<std::string, TFunction> functions,
                  std::map<std::string, TIterator> iteratorsNameAccess);
