#include "../raylib/src/raylib.h"
#include "../utils/utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <map>
#include <tuple>

using namespace std;

int parseRun(string fileName)
{
    map<string, tuple<string, string>> programVars;
    map<int, tuple<string, int, int, int>> iterators;
    map<string, tuple<int, int, int>> iteratorsNameAccess;
    // First tuple int is the value of
    // the iterator, second and third is start and stop. String is the name. Key is the
    // nested number.

    const vector<string> varTypes = {"bool", "int", "float", "string", "list", "iterator"};
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
            iteratorsNameAccess[get<0>(iterators[j])] = tuple<int, int, int>{get<1>(iterators[j]), get<2>(iterators[j]), get<3>(iterators[j])};
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
                        else
                        {
                            continue;
                        }
                    }
                    else // forever
                    {
                        if (get<1>(iterators[nestedStatements]) == -1)
                        {
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
            string varVal = splitEquals[1];

            if (startsWith(varVal, " "))
            {
                varVal = varVal.substr(1, varVal.size() - 1);
            }

            if (find(varTypes.begin(), varTypes.end(), varType) == varTypes.end())
            {
                cout << "Error: Unknown type '" << varType << "' on line " << i + 1 - linesImported << ", aborting." << endl
                        << flush;
                return 1;
            }
            if (trimChar(varType, ' ') == "iterator")
            {
                cout << "TypeError on line " << i + 1 - linesImported << ": Cannot create variable of iterator type. Aborting." << flush;
                return 1;
            }

            tuple<string, string> varDesc = {trimChar(varType, ' '), trimChar(varVal, ' ')};
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
                cout << endl
                        << flush;
                continue;
            }
            if (splitSpaces[1] == "var")
            {
                cout << get<1>(programVars[splitSpaces[2]]) << flush;
                continue;
            }
            else
            {
                cout << splitString(splitLines[i], "coutstreamadd ")[1] << flush;
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
            string leftVal;
            string rightVal;

            if (leftSplitSpace[0] == "var")
            {
                leftVal = get<1>(programVars[leftSplitSpace[1]]);
                string type = get<0>(programVars[leftSplitSpace[1]]);

                if ((type == "string" | type == "list" | type == "bool") && !(operatorUsed == "==" | operatorUsed == "!="))
                {
                    cout << "TypeError: On line " << i + 1 - linesImported << ", left value of type '" << type << "' cannot use the '" << operatorUsed << "' operator." << endl
                            << flush;
                    return 1;
                }
            }
            else if (leftSplitSpace[0] == "it")
            {
                tuple<int, int, int> leftValMap = iteratorsNameAccess[leftSplitSpace[1]];
                int leftValInt = get<0>(leftValMap);
                leftVal = to_string(leftValInt);
            }
            else
            {
                leftVal = leftSplitSpace[0];
            }

            if (rightSplitSpace[0] == "var")
            {
                rightVal = get<1>(programVars[rightSplitSpace[1]]);
                string type = get<0>(programVars[leftSplitSpace[1]]);

                if ((type == "string" | type == "list" | type == "bool") && !(operatorUsed == "==" | operatorUsed == "!="))
                {
                    cout << "TypeError: On line " << i + 1 - linesImported << ", right value of type '" << type << "' cannot use the '" << operatorUsed << "' operator." << endl
                            << flush;
                    return 1;
                }
            }
            else if (rightSplitSpace[0] == "it")
            {
                rightVal = to_string(get<0>(iteratorsNameAccess[rightSplitSpace[1]]));
            }
            else
            {
                rightVal = rightSplitSpace[0];
            }

            leftVal = trimChar(leftVal, ' ');
            rightVal = trimChar(rightVal, ' ');

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
                ifReturns = stof(leftVal) > stof(rightVal);
            }
            if (operatorUsed == "<")
            {
                ifReturns = stof(leftVal) < stof(rightVal);
            }
            if (operatorUsed == ">=")
            {
                ifReturns = stof(leftVal) >= stof(rightVal);
            }
            if (operatorUsed == "<=")
            {
                ifReturns = stof(leftVal) <= stof(rightVal);
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
            iterators[nestedStatements] = tuple<string, int, int, int>{iteratorName, iteratorStart + 1, iteratorStart + 1, iteratorEnd};
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
            while (!startsWith(splitLines[i], "}") | bracketExceptions > 0)
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
            programVars[splitSpaces[1]] = tuple<string, string>{"string", inputString};
        }

        if (startsWith(splitLines[i], (string){"quit"}))
        {
            return 0;
        }
    }

    return 0;
}