#include "../raylib/src/raylib.h"
#include "../utils/utils.h"
#include "listcomp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <map>
#include <tuple>

using namespace std;

// Yucky code not mine ------
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
// end of yucky code --------

// I would hide this function if I were you ...
TVarObj getVarValNoVariableIndexes(string var, map<string, TVarObj> programVars)
{
    string numString;
    if (startsWith(var, "["))
    {
        for (int i = 0; i < var.size(); i++)
        {
            if (var[i] == '[')
            {
                continue;
            }
            if (var[i] == ']')
            {
                break;
            }
            else 
            {
                numString += var[i];
            }
        }
    }
    else 
    {
        numString = var;
    }

    if (programVars.find(numString) != programVars.end())
    {
        return programVars[var];
    }
    cout << "Warning in Variable Comprehension: Variable lookup failed." << endl << flush;
    TVarObj nullObj;
    return nullObj;
}

string convertVarToStringList(std::variant<TListItem, TList> input, map<string, TVarObj> programVars)
{
    string out;
    visit(
        overloaded{
            [&](TListItem &var)
            {
                visit(
                    overloaded{
                        [&](bool &itemVar)
                        {
                            out = to_string(itemVar);
                        },
                        [&](int &itemVar)
                        {
                            out = to_string(itemVar);
                        },
                        [&](float &itemVar)
                        {
                            out = to_string(itemVar);
                        },
                        [&](string &itemVar)
                        {
                            out = itemVar;
                        },
                        [&](TIterator &itemVar)
                        {
                            out = to_string(get<1>(itemVar));
                        },
                        [&](TPointer &itemVar)
                        {
                            out = "EmbededPointer";
                        }
                    },
                    var
                );
            },
            [&](TList &var)
            {
                out += "[";
                for (int i = 0; i < var.item.size(); i++)
                {
                    out += convertVarToStringList(var.item[i], programVars);
                    out += ",";
                }
                if (var.item.size() > 0)
                {
                    out.erase(out.end() - 1);
                }
                out += "]";
            }},
        input);
    return out;
}

string convertVarToString(TVarObj input, map<string, TVarObj> programVars)
{
    string out;
    std::visit(
        overloaded{
            [&](bool &var)
            {
                out = to_string(var);
            },
            [&](int &var)
            {
                out = to_string(var);
            },
            [&](float &var)
            {
                out = to_string(var);
            },
            [&](string &var)
            {
                out = var;
            },
            [&](TPointer &var)
            {
                out = convertVarToString(getVarValNoVariableIndexes(get<1>(var), programVars), programVars);
            },
            [&](TIterator &var)
            {
                out = to_string(get<1>(var));
            },
            [&](TList &var)
            {
                out += "[";
                for (int i = 0; i < var.item.size(); i++)
                {
                    out += convertVarToStringList(var.item[i], programVars);
                    out += ",";
                }
                if (var.item.size() > 0)
                {
                    out.erase(out.end() - 1);
                }
                out += "]";
            }},
        input);

    return out;
}

string getListBaseType(TVarObj listVar)
{
    string out;
    visit(
        overloaded{
            [&](TList &var)
            {
                vector<string> splitColon = splitString(var.itemType, ":");
                out = splitColon[splitColon.size() - 1];
            },
            [&](bool) {},
            [&](int) {},
            [&](float) {},
            [&](string) {},
            [&](TPointer) {},
            [&](TIterator) {},
        },
        listVar);

    return out;
}

string getListItemType(TVarObj listVar)
{
    string out;
    visit(
        overloaded{
            [&](TList &var)
            {
                vector<string> splitColon = splitString(var.itemType, ":");
                for (int i = 1; i < splitColon.size(); i++)
                {
                    out += splitColon[i];
                    out += ":";
                }
                out.erase(out.end() - 1);
            },
            [&](bool) {},
            [&](int) {},
            [&](float) {},
            [&](string) {},
            [&](TPointer) {},
            [&](TIterator) {},
        },
        listVar);

    return out;
}

variant<TListItem, TList> getListItemFromVar(string varType, string varValue)
{
    variant<TListItem, TList> output;

    if (varType == "string")
    {
        output = varValue;
    }
    else if (varType == "int")
    {
        output = stoi(varValue);
    }
    else if (varType == "float")
    {
        output = stof(varValue);
    }
    else if (varType == "bool")
    {
        if (varValue == "true" || varValue == "1")
        {
            output = true;
        }
        if (varValue == "false" || varValue == "0")
        {
            output = false;
        }
    }
    else if (varType == "var")
    {
        output = (TPointer){varType, varValue};
    }
    else if (startsWith(varType, "list:"))
    {
        TList outputList;
        vector<string> unpackedStringList = unpackList(varValue);
        string listValueType = trimString(varType, "list:");

        for (int i = 0; i < unpackedStringList.size(); i++)
        {
            std::variant<TListItem, TList> item = getListItemFromVar(listValueType, unpackedStringList[i]);
            outputList.item.push_back(item);
        }

        outputList.itemType = varType;
        output = outputList;
    }   
    else 
    {
        cout << "Warning in Variable Comprehension: Invalid list variable type." << endl << flush;
    }

    return output;
}

TVarObj getMappableVar(string varType, string varValue)
{
    TVarObj output;
    if (varType == "string")
    {
        output = varValue;
    }
    else if (varType == "int")
    {
        output = stoi(varValue);
    }
    else if (varType == "float")
    {
        output = stof(varValue);
    }
    else if (varType == "bool")
    {
        if (varValue == "true" || varValue == "1")
        {
            output = true;
        }
        if (varValue == "false" || varValue == "0")
        {
            output = false;
        }
    }
    else if (varType == "var")
    {
        output = (TPointer){varType, varValue};
    }
    else if (startsWith(varType, "list"))
    {
        TList outputList;
        vector<string> unpackedStringList = unpackList(varValue);
        string listValueType = trimString(varType, "list:");

        for (int i = 0; i < unpackedStringList.size(); i++)
        {
            variant<TListItem, TList> item = getListItemFromVar(listValueType, unpackedStringList[i]);
            outputList.item.push_back(item);
        }

        outputList.itemType = varType;
        output = outputList;
    }
    else 
    {
        cout << "Warning in Variable Comprehension: Unable to create variable." << endl << flush;
    }

    return output;
}

TVarObj getVarVal(string var, map<string, TVarObj> programVars)
{
    string numString;
    if (startsWith(var, "["))
    {
        string valNoBracket;
        bool addToNoBracket;

        for (int i = 0; i < var.size(); i++)
        {
            if (addToNoBracket)
            {
                valNoBracket += var[i];
            }
            if (var[i] == '[')
            {
                continue;
            }
            if (var[i] == ']')
            {
                addToNoBracket = true;
            }
            else 
            {
                if (!addToNoBracket)
                {
                    numString += var[i];
                }
            }
        }

        if (startsWith(numString, "var"))
        {
            TVarObj numObj = getVarVal(splitString(numString, ":")[1], programVars);
            numString = convertVarToString(numObj, programVars);
        }

        TVarObj listVal = getVarVal(valNoBracket, programVars);
        string listString = unpackList(convertVarToString(listVal, programVars))[stoi(numString)];
        string baseType = getListItemType(listVal);
        TVarObj objectVal = getMappableVar(baseType, listString);
        return objectVal;
    }
    else 
    {
        numString = var;
    }

    if (programVars.find(numString) != programVars.end())
    {
        return programVars[var];
    }
    cout << "Warning in Variable Comprehension: Variable lookup failed." << endl << flush;
    TVarObj nullObj;
    return nullObj;
}

TVarObj getPointerVal(TPointer pointer, map<string, TIterator> iteratorsNameAccess, map<string, TVarObj> programVars)
{
    string name = get<1>(pointer);
    if (programVars.contains(name))
    {
        return programVars[name];
    }
    else if (iteratorsNameAccess.contains(name))
    {
        return iteratorsNameAccess[name];
    }
    else 
    {
        cout << "AttributeError: Pointer '" << name << "' points to void. Dumping." << endl << flush;
        exit(EXIT_FAILURE);
    }
}
