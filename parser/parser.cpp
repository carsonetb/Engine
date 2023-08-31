#include "../raylib/src/raylib.h"
#include "../utils/utils.h"
#include "listcomp.h"
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

int parseRun(string fileName)
{
    // General variables, varies.
    map<string, TVarObj> programVars;

    // Iterators, used in for loops.
    map<int, TIterator> iterators;

    // Sometimes we need to access an iterator by it's name, not it's nested
    // depth.
    map<string, TIterator> iteratorsNameAccess;

    // Types of variables that can be created. Not all of them can be 
    // specifically created by the user.
    const vector<string> varTypes = {"bool", "int", "float", "string", "list", "iterator", "pointer"};

    // Operators used to compare things.
    const vector<string> compareOperators = {"==", "!=", ">", "<", ">=", "<="};

    string fileString = readFile(fileName);
    vector<string> splitLines = splitString(fileString, "\n");
    int nestedStatements = 0;

    // Create map for storing nested statement info for functions, if statements,
    // for loops, while loops, etc. Key is the nesting number, value is tuple of
    // possible information. 1 holds the type (i.e. "while", "func", or "if").
    // 2 holds the line a function was called from or the line of the start of 
    // a for or while loop. 3 holds if an if statement is true or false.
    map<int, tuple<string, int, bool>> statementInfo;

    bool justCompletedIf = false;
    bool justcompletedIfInfo = false;

    int linesImported = 0;

    for (int i = 0; i < splitLines.size(); i++)
    {
        splitLines[i] = removeAllFirstChars(splitLines[i], " ");
    }

    for (int i = 0; i < splitLines.size(); i++)
    {
        for (int j = 0; j < iterators.size(); j++)
        {
            iteratorsNameAccess[get<0>(iterators[j])] = iterators[j];
        }

        if (startsWith(splitLines[i], (string) "}"))
        {
            if (nestedStatements > 0)
            {
                if (get<0>(statementInfo[nestedStatements]) == "if")
                {
                    justCompletedIf = true;
                    justcompletedIfInfo = get<2>(statementInfo[nestedStatements]);
                    statementInfo.erase(nestedStatements);
                    nestedStatements -= 1;
                    continue;
                }
                if (get<0>(statementInfo[nestedStatements]) == "for")
                {
                    if (!get<2>(statementInfo[nestedStatements]))
                    {
                        if (get<1>(iterators[nestedStatements]) < get<3>(iterators[nestedStatements]))
                        {
                            get<1>(iterators[nestedStatements]) += 1;
                            i = get<1>(statementInfo[nestedStatements]);
                            continue;
                        }
                        else // end
                        {
                            iterators.erase(nestedStatements);
                            continue;
                        }
                    }
                    else // forever
                    {
                        if (get<1>(iterators[nestedStatements]) == -1)
                        {
                            iterators.erase(nestedStatements);
                            continue;
                        }

                        get<1>(iterators[nestedStatements]) += 1;
                        i = get<1>(statementInfo[nestedStatements]);
                        continue;
                    }
                }
            }
            else
            {
                cout << "SyntaxError: Unexpected '}' on line " << i + 1 - linesImported << ". Aborting." << flush;
                return 1;
            }
        }

        if (!get<2>(statementInfo[nestedStatements]) && get<0>(statementInfo[nestedStatements]) == "if")
        {
            justCompletedIf = false;
            justcompletedIfInfo = false;
            continue;
        }

        if (startsWith(splitLines[i], (string) "var"))
        {
            justCompletedIf = false;
            justcompletedIfInfo = false;
            vector<string> splitEquals = splitString(splitLines[i], "=");

            vector<string> beforeEqualsSplitSpace = splitString(splitEquals[0], " ");
            string varType = beforeEqualsSplitSpace[1]; // var <<[type]>> [name] = [val];
            string varName = beforeEqualsSplitSpace[2]; // var [type] <<[name]>> = [val];
            string varVal = splitEquals[1]; // var [type] [name] = <<[val]>>

            TVarObj varDesc = getMappableVar(varType, varVal);
            programVars[varName] = varDesc;
            continue;
        }

        if (startsWith(splitLines[i], (string) "coutstreamadd"))
        {
            justCompletedIf = false;
            justcompletedIfInfo = false;
            vector<string> splitSpaces = splitString(splitLines[i], " ");

            if (splitSpaces[1] == "endl")
            {
                cout << endl << flush;
                continue;
            }
            if (splitSpaces[1] == "var")
            {
                TVarObj varObj = getVarVal(splitSpaces[2], programVars);
                string varString = convertVarToString(varObj, programVars);
                cout << varString << flush;
                continue;
            }
            else
            {
                string toPrint = splitString(splitLines[i], "coutstreamadd ")[1];
                cout << toPrint << flush;
                continue;
            }
        }

        if (startsWith(splitLines[i], (string) "using"))
        {
            justCompletedIf = false;
            justcompletedIfInfo = false;
            vector<string> splitSpaces = splitString(splitLines[i], " ");
            string file = readFile(splitSpaces[1]);
            vector<string> splitFile = splitString(file, "\n");
            splitLines = insertVectorAtIndString(i + 1 - linesImported, splitLines, splitFile);
            linesImported += splitFile.size();
            continue;
        }

        if (startsWith(splitLines[i], (string) "if"))
        {
            justCompletedIf = false;
            justcompletedIfInfo = false;
            vector<string> splitEquals = splitString(splitLines[i], "==");

            int j = -1;
            string operatorUsed = "==";
            while (splitEquals.size() != 2)
            {
                j++;
                splitEquals = splitString(splitLines[i], compareOperators[j]);
                operatorUsed = compareOperators[j];
            }

            string left = splitString(splitEquals[0], "if ")[1];
            string right = splitString(splitEquals[1], "{")[0];
            left = trimChar(left, ' ');
            right = trimChar(right, ' ');
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

            nestedStatements += 1;
            statementInfo[nestedStatements] = tuple<string, int, bool>{"if", -1, ifReturns};
            continue;
        }

        if (startsWith(splitLines[i], (string) "else"))
        {
            if (justCompletedIf)
            {
                if (!justcompletedIfInfo)
                {
                    nestedStatements += 1;
                    statementInfo[nestedStatements] = tuple<string, int, bool>{"if", -1, true};
                }
            }
            else
            {
                cout << "SyntaxError: Unexpected 'else' on line " << i + 1 - linesImported << "." << endl
                        << flush;
                return 1;
            }

            justCompletedIf = false;
            justcompletedIfInfo = false;
            continue;
        }

        if (startsWith(splitLines[i], (string) "for"))
        {
            string removedBracket = splitString(splitLines[i], "{")[0];
            vector<string> splitSpace = splitString(removedBracket, " ");
            string iteratorName = splitSpace[1];
            int iteratorEnd;
            bool forever = false;

            if (splitSpace[3] == ":")
            {
                if (splitSpace[2] == "-1")
                {
                    cout << "Warning: The infinite for loop on line " << i + 1 - linesImported << " should not have a start option of -1." << endl
                            << flush;
                }

                iteratorEnd = -1;
                forever = true;
            }
            else
            {
                int iteratorEnd = stoi(splitSpace[3]);
            }

            int iteratorStart = stoi(splitSpace[2]);

            nestedStatements += 1;
            iterators[nestedStatements] = (TIterator){iteratorName, iteratorStart + 1, iteratorStart + 1, iteratorEnd};
            statementInfo[nestedStatements] = tuple<string, int, bool>{"for", i, forever};
            continue;
        }

        if (startsWith(splitLines[i], (string) "break"))
        {
            // Check if break is inside function, if there is no for loop in that
            // function, error.
            get<1>(iterators[iterators.size() - 1]) = get<3>(iterators[iterators.size() - 1]);
            int bracketExceptions = 0;
            while (get<0>(statementInfo[nestedStatements]) != "for")
            {
                bracketExceptions += 1;
                nestedStatements -= 1;
            }
            while (!startsWith(splitLines[i], "}") || bracketExceptions > 0)
            {
                if (startsWith(splitLines[i], "}"))
                {
                    bracketExceptions -= 1;
                }
                i++;
            }
            i -= 1;
        }

        if (startsWith(splitLines[i], (string) "input"))
        {
            string inputString;
            cin >> inputString;
            vector<string> splitSpaces = splitString(splitLines[i], " ");
            programVars[splitSpaces[1]] = inputString;
        }

        if (startsWith(splitLines[i], (string){"quit"}))
        {
            return 0;
        }
    }

    return 0;
}