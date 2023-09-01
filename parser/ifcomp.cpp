// If Statement (kwd: if) Computation
// --------------------------------------------------

// Library for handling and computing if statements
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

// if (int 5 == int 5 && int 4 == int 4) || ((int 5 == 5 || int 4 == int 5) && int 6 == int 7) {true}
// if int 5 == int 4 || int 5 == int 5 && int 7 > int 5 {true}

vector<vector<string>> segmentateIf(string statement)
{
    vector<vector<string>> out;
    vector<string> splitAnd = splitString(statement, "&&");

    for (int i = 0; i < splitAnd.size(); i++)
    {
        vector<string> splitOr = splitString(splitAnd[i], "||");
        out.push_back(splitOr);
    }

    return out;
}

bool isCheckTrue(string check, vector<string> compareOperators, map<string, TVarObj> programVars, map<string, TIterator> iteratorsNameAccess, int i, int linesImported)
{
    vector<string> splitEquals = splitString(check, "==");

    int j = -1;
    string operatorUsed = "==";
    while (splitEquals.size() != 2)
    {
        j++;
        splitEquals = splitString(check, compareOperators[j]);
        operatorUsed = compareOperators[j];
    }

    string left = trimChar(splitEquals[0], ' ');
    string right = trimChar(splitEquals[1], ' ');
    vector<string> leftSplitSpace = splitString(left, " ");
    vector<string> rightSplitSpace = splitString(right, " ");
    TVarObj leftVal;
    TVarObj rightVal;

    if (leftSplitSpace[0] == "var")
    {
        leftVal = programVars[leftSplitSpace[1]];
    }
    else if (leftSplitSpace[0] == "it")
    {
        leftVal = iteratorsNameAccess[leftSplitSpace[1]];
    }
    else
    {
        leftVal = getMappableVar(leftSplitSpace[0], leftSplitSpace[1]);
    }

    if (rightSplitSpace[0] == "var")
    {
        rightVal = programVars[rightSplitSpace[1]];
    }
    else if (rightSplitSpace[0] == "it")
    {
        rightVal = iteratorsNameAccess[rightSplitSpace[1]];
    }
    else
    {
        rightVal = getMappableVar(rightSplitSpace[0], rightSplitSpace[1]);
    }

    string leftStringType = TVarObjTypes.at((int)leftVal.index());
    string rightStringType = TVarObjTypes.at((int)rightVal.index());
    if (((leftStringType == "string" | leftStringType == "list" | leftStringType == "bool") 
        | (rightStringType == "string" | rightStringType == "list" | rightStringType == "bool"))
        && !(operatorUsed == "==" | operatorUsed == "!=")
    )
    {
        cout << "SyntaxError on line " << i + 1 - linesImported << ": In valid operands for operator '" << operatorUsed << "'. Aborting." << endl << flush;
        return 1;
    }

    bool ifReturns = false;

    if (operatorUsed == "==")
    {
        ifReturns = leftVal == rightVal;
    }
    if (operatorUsed == "!=")
    {
        ifReturns = leftVal != rightVal;
    }
    if (operatorUsed == ">")
    {
        ifReturns = leftVal > rightVal;
    }
    if (operatorUsed == "<")
    {
        ifReturns = leftVal < rightVal;
    }
    if (operatorUsed == ">=")
    {
        ifReturns = leftVal >= rightVal;
    }
    if (operatorUsed == "<=")
    {
        ifReturns = leftVal <= rightVal;
    }

    return ifReturns;
}

bool checkSegmentatedIf(vector<vector<string>> toCheck, vector<string> compareOperators, map<string, TVarObj> programVars, map<string, TIterator> iteratorsNameAccess, int i, int linesImported)
{
    for (int i = 0; i < toCheck.size(); i++)
    {
        bool orCheck = false;

        for (int j = 0; j < toCheck[i].size(); j++)
        {
            if (isCheckTrue(toCheck[i][j], compareOperators, programVars, iteratorsNameAccess, i, linesImported))
            {
                orCheck = true;
                break;
            }
        }

        if (!orCheck)
        {
            return false;
        }
    }

    return true;
}