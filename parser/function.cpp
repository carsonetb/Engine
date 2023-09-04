// Function Computation
// --------------------------------------------------

// Library for parsing functions.
// Header: parser/ifcomp.h

#include "../raylib/src/raylib.h"
#include "../utils/utils.h"
#include "varcomp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <map>
#include <tuple>
#include <variant>

using namespace std;

// func int testFunc(int a, int b) {
//    return a + b
// }
// callfunc testFunc(5, 10) into added

vector<tuple<string, string>> unpackArgumentString(string arguments)
{
    vector<string> splitComma = splitString(arguments, ",");
    vector<tuple<string, string>> out;

    for (int i = 0; i < splitComma.size(); i++)
    {
        string trimmedArgument = trimString(splitComma[i], " ");
        vector<string> splitSpace = splitString(trimmedArgument, " ");

        string argType = splitSpace[0];
        string argName = splitSpace[1];

        out.push_back(tuple<string, string>{argType, argName});
    }

    return out;
}

vector<tuple<bool, string>> unpackCallArgumentString(string arguments)
{
    vector<string> splitComma = splitString(arguments, ",");
    vector<tuple<bool, string>> out;

    for (int i = 0; i < splitComma.size(); i++)
    {
        string trimmedArgument = trimString(splitComma[i], " ");
        vector<string> splitSpace = splitString(trimmedArgument, " ");

        if (startsWith(splitSpace[0], "var"))
        {
            out.push_back({true, splitSpace[1]});
        }
        else 
        {
            out.push_back({false, trimmedArgument});
        }
    }

    return out;
}

TFunction unpackFunctionLine(string line, int currentLine)
{
    TFunction out;
    out.createdLine = currentLine;

    string removedCurlyBracket = splitString(line, "{")[0];
    string removedCloseParenthasis = splitString(removedCurlyBracket, ")")[0];
    vector<string> splitOpenParenthasis = splitString(removedCloseParenthasis, "(");
    string argumentString = splitOpenParenthasis[1];
    string callString = splitOpenParenthasis[0];

    vector<string> splitSpace = splitString(callString, " ");
    out.returnType = splitSpace[1];
    out.name = splitSpace[2];

    out.arguments = unpackArgumentString(argumentString);

    return out;
}

TFunctionCall unpackFunctionCallLine(string line)
{
    TFunctionCall out;

    string removedCloseParenthasis = splitString(line, ")")[0];
    vector<string> splitOpenParenthasis = splitString(removedCloseParenthasis, "(");
    string argumentString = splitOpenParenthasis[1];
    string callString = splitOpenParenthasis[0];

    vector<string> splitSpace = splitString(callString, " ");
    out.name = splitSpace[1];
    out.arguments = unpackCallArgumentString(argumentString);

    string returnVarName = splitString(line, " into ")[1];

    if (!startsWith(returnVarName, "callfunc "))
    {
        out.returnVarName = returnVarName;
    }
    else 
    {
        out.returnVarName = "void";
    }

    return out;
}

void callFunction(TFunctionCall toCall, map<int, tuple<string, int, bool, string>> &statementInfo, map<string, TVarObj> &programVars, 
                  int &currentLine, int &nestedStatements, map<string, TFunction> functions, map<string, TIterator> iteratorsNameAccess)
{
    statementInfo[nestedStatements + 1] = tuple<string, int, bool, string>{(string) "func", currentLine, false, toCall.name};
    currentLine = functions[toCall.name].createdLine;
    nestedStatements++;

    for (int i = 0; i < toCall.arguments.size(); i++)
    {
        string argVal = get<1>(toCall.arguments[i]);
        string argName = get<1>(functions[toCall.name].arguments[i]);
        string argType = get<0>(functions[toCall.name].arguments[i]);
        bool isVar = get<0>(toCall.arguments[i]);
        programVars[argName] = isVar ? getVarVal(argVal, programVars, iteratorsNameAccess) : getMappableVar(argType, argVal);
    }
}