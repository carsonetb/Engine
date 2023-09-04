// T Language Interpretter
// --------------------------------------------------

// T parser, central handler library for interpretting.
// Header: parser/parser.h

#include "../raylib/src/raylib.h"
#include "../utils/utils.h"
#include "listcomp.h"
#include "varcomp.h"
#include "ifcomp.h"
#include "function.h"
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

// Interpret mode: Parse and run program.
int parseRun(string fileName)
{
    // General variables, varies.
    map<string, TVarObj> programVars;

    // Iterators, used in for loops.
    map<int, TIterator> iterators;

    // Sometimes we need to access an iterator by it's name, not it's nested
    // depth.
    map<string, TIterator> iteratorsNameAccess;

    // Map of functions. Can be accessed by name of function.
    map<string, TFunction> functions;

    // Temporary variable names for inside functions, accessed by depth.
    map<int, vector<string>> tempProgramVars;

    // Types of variables that can be created. Not all of them can be 
    // specifically created by the user.
    const vector<string> varTypes = {"bool", "int", "float", "string", "list", "iterator", "pointer"};

    // Operators used to compare things.
    const vector<string> compareOperators = {"==", "!=", ">", "<", ">=", "<="};

    // Operators used to change things.
    const vector<string> changeOperators = {"+", "-", "*", "/", "+=", "-=", "*=", "/="};

    string fileString = readFile(fileName);
    vector<string> splitLines = splitString(fileString, "\n");
    int nestedStatements = 0;

    // Create map for storing nested statement info for functions, if statements,
    // for loops, while loops, etc. Key is the nesting number, value is tuple of
    // possible information. 1 holds the type (i.e. "for", "func", or "if").
    // 2 holds the line a function was called from or the line of the start of 
    // a for or while loop. 3 holds if an if statement is true or false. 4 holds
    // a function's name.
    map<int, tuple<string, int, bool, string>> statementInfo;

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
                if (get<0>(statementInfo[nestedStatements]) == "func")
                {
                    for (int i = 0; i < tempProgramVars[nestedStatements].size(); i++)
                    {
                        programVars.erase(tempProgramVars[nestedStatements][i]);
                    }

                    if (functions[get<3>(statementInfo[nestedStatements])].returnType != "void")
                    {
                        cout << "NoReturnError on line " << i + 1 - linesImported << ". End of function reached but function does not return 'void'. Aborting." << endl << flush;
                        return 1;
                    }

                    i = get<1>(statementInfo[nestedStatements]);
                    tempProgramVars.erase(nestedStatements);
                    nestedStatements -= 1;
                    continue;
                }
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
                        if (get<1>(iterators[nestedStatements]) < get<3>(iterators[nestedStatements]) - 1)
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

        if (startsWith(splitLines[i], (string) "func"))
        {
            justCompletedIf = false;
            justcompletedIfInfo = false;

            TFunction createdFunction = unpackFunctionLine(splitLines[i], i);
            functions[createdFunction.name] = createdFunction;

            int bracketDepth = 0;
            i++;

            while (bracketDepth > 0 || !startsWith(splitLines[i], "}"))
            {
                if (startsWith(splitLines[i], "for") || startsWith(splitLines[i], "if") || startsWith(splitLines[i], "func"))
                {
                    bracketDepth += 1;
                }

                if (startsWith(splitLines[i], "}"))
                {
                    bracketDepth -= 1;
                }

                i++;
            }

            continue;
        }

        if (startsWith(splitLines[i], (string) "callfunc"))
        {
            TFunctionCall toCall = unpackFunctionCallLine(splitLines[i]);
            callFunction(toCall, statementInfo, programVars, i, nestedStatements, functions, iteratorsNameAccess);
            continue;
        }

        if (startsWith(splitLines[i], (string) "return"))
        {
            while (get<0>(statementInfo[nestedStatements]) != "func")
            {
                nestedStatements -= 1;

                if (nestedStatements == 0)
                {
                    cout << "SyntaxError on line " << i + 1 - linesImported << ": Unexpected 'return' when not inside function. Aborting." << endl << flush;
                    return 1;
                }
            }

            TFunction calledFunctionVar = functions[get<3>(statementInfo[nestedStatements])];
            int calledLine = get<1>(statementInfo[nestedStatements]);
            TFunctionCall calledFunction = unpackFunctionCallLine(splitLines[calledLine]);
            string returnVarName = calledFunction.returnVarName;

            vector<string> splitSpace = splitString(splitLines[i], " ");
            TVarObj returnValue;

            if (splitSpace[1] == "var")
            {
                returnValue = getVarVal(splitSpace[2], programVars, iteratorsNameAccess);
            }
            else 
            {
                returnValue = getMappableVar(calledFunctionVar.returnType, splitSpace[1]);
            }

            if (returnVarName != "void")
            {
                programVars[returnVarName] = returnValue;
            }

            i = calledLine;
            continue;
        }

        if (startsWith(splitLines[i], (string) "varchange"))
        {
            justCompletedIf = false;
            justcompletedIfInfo = false;
            string removedKwd = splitString(splitLines[i], "varchange ")[1];
            vector<string> splitOperator;

            int j = -1;
            string operatorUsed = "+";
            while (splitOperator.size() != 2)
            {
                j++;
                splitOperator = splitString(removedKwd, changeOperators[j]);
                operatorUsed = changeOperators[j];
            }

            string left = splitOperator[0];
            string right = splitOperator[1];
            left = trimString(left, " ");
            right = trimString(right, " ");
            TVarObj leftObj;
            TVarObj rightObj;

            leftObj = getVarVal(left, programVars, iteratorsNameAccess);

            if (startsWith(right, "var"))
            {
                string rightRemovedVar = splitString(right, "var ")[1];
                rightObj = getVarVal(rightRemovedVar, programVars, iteratorsNameAccess);
            }
            else 
            {
                vector<string> splitSpace = splitString(right, " ");
                rightObj = getMappableVar(splitSpace[0], splitSpace[1]);
            }

            TVarObj out;

            if (operatorUsed == "+")
            {
                out = addObjs(leftObj, rightObj);
            }
            if (operatorUsed == "-")
            {
                out = subtractObjs(leftObj, rightObj);
            }
            if (operatorUsed == "*")
            {
                out = multiplyObjs(leftObj, rightObj);
            }
            if (operatorUsed == "/")
            {
                out = divideObjs(leftObj, rightObj);
            }

            programVars[left] = out;
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

        if (startsWith(splitLines[i], (string) "setvar"))
        {
            vector<string> splitSpace = splitString(splitLines[i], " ");
            string varToSet = splitSpace[1];
            string varToSetTo = splitSpace[2];
            TVarObj varToSetToObj = getVarVal(varToSetTo, programVars, iteratorsNameAccess);
            programVars[varToSet] = varToSetToObj;
        }

        if (startsWith(splitLines[i], (string) "print"))
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
                TVarObj varObj = getVarVal(splitSpaces[2], programVars, iteratorsNameAccess);
                string varString = convertVarToString(varObj, programVars, iteratorsNameAccess);
                cout << varString << flush;
                continue;
            }
            else
            {
                string toPrint = splitString(splitLines[i], "print ")[1];
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

            string ifRemoved = splitString(splitLines[i], "if ")[1];
            string curlyBracketRemoved = splitString(ifRemoved, "{")[0];
            vector<vector<string>> segmentatedIf = segmentateIf(curlyBracketRemoved);
            bool ifReturns = checkSegmentatedIf(segmentatedIf, compareOperators, programVars, iteratorsNameAccess, i, linesImported);

            nestedStatements += 1;
            statementInfo[nestedStatements] = tuple<string, int, bool, string>{"if", -1, ifReturns, ""};
            continue;
        }

        if (startsWith(splitLines[i], (string) "else"))
        {
            if (justCompletedIf)
            {
                if (!justcompletedIfInfo)
                {
                    nestedStatements += 1;
                    statementInfo[nestedStatements] = tuple<string, int, bool, string>{"if", -1, true, ""};
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
                iteratorEnd = stoi(splitSpace[3]);
            }

            int iteratorStart = stoi(splitSpace[2]) - 1;

            nestedStatements += 1;
            iterators[nestedStatements] = (TIterator){iteratorName, iteratorStart + 1, iteratorStart + 1, iteratorEnd};
            statementInfo[nestedStatements] = tuple<string, int, bool, string>{"for", i, forever, ""};
            continue;
        }

        if (startsWith(splitLines[i], (string) "break") || startsWith(splitLines[i], (string) "continue"))
        {
            // Check if break is inside function, if there is no for loop in that
            // function, error.
            if (startsWith(splitLines[i], (string) "break"))
            {
                get<1>(iterators[iterators.size() - 1]) = get<3>(iterators[iterators.size() - 1]);
            }
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

        if (startsWith(splitLines[i], (string)"quit"))
        {
            return 0;
        }

        if (startsWith(splitLines[i], (string)"throwError"))
        {
            cout << "UserError: User created error occured. Aborting." << endl << flush;
            return 3;
        }
    }

    return 0;
}