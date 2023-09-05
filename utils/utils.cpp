#include "utils.h"
#include "../raylib/src/raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <map>
#include <tuple>
#include <variant>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>

using namespace std;

// Yucky code not mine ------
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
// end of yucky code --------

void si(string &i)
{
    i = stoi(i);
}

void sf(string &i)
{
    i = stof(i);
}

// trim from start (in place)
string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// trim from end (in place)
string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// trim from both ends (in place)
string trim(std::string s) {
    s = rtrim(s);
    s = ltrim(s);
    return s;
}

string trimChar(string s, char toTrim)
{
    if (s[0] == toTrim)
    {
        s = ltrim(s);
    }
    if (s[-1] == toTrim)
    {
        s = rtrim(s);
    }
    return s;
}

string readFile(string fileName)
{
    ifstream file;
    file.open(fileName);

    string output;

    if (file.is_open())
    {
        while (file)
        {
            int temp = file.get();
            if (file.eof())
            {
                break;
            }
            output += temp;
        }
    }
    else 
    {
        cout << "IOError: File couldn't be opened. Killing program >:)" << endl;
        
        // Super secret code-killer >>>:)))
        exit(EXIT_FAILURE);
    }

    return output;
}

vector<string> splitString(string toSplit, string character)
{
    vector<string> result;
    size_t i = toSplit.find(character);
    result.push_back(toSplit.substr(0, i));

    if (i == string::npos)
    {
        return result;
    }

    while (true)
    {
        i += character.size();
        size_t end = toSplit.find(character, i);
        string rest = toSplit.substr(i, end - i);
        i += rest.size();
        result.push_back(rest);

        if (end == string::npos)
        {
            break;
        }
    }

    return result;
}

bool startsWith(const string &str, const string &prefix) 
{
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

bool endsWith(const string &str, const string &suffix) 
{
    if (str.length() >= suffix.length()) {
        return (0 == str.compare (str.length() - suffix.length(), suffix.length(), suffix));
    } else {
        return false;
    }
}

string trimString(string toTrim, string splitBy)
{
    if (startsWith(toTrim, splitBy))
    {
        for (int i = 0; i < splitBy.size(); i++)
        {
            toTrim.erase(toTrim.begin());
        }
    }
    if (endsWith(toTrim, splitBy))
    {
        for (int i = 0; i < splitBy.size(); i++) 
        {
            toTrim.erase(toTrim.end() - 1);
        }
    }

    return toTrim;
}

vector<string> insertVectorAtIndString(int i, vector<string> addedInto, vector<string> toAdd)
{
    // Split at index.
    vector<string> split_lo(addedInto.begin(), addedInto.begin() + i);
    vector<string> split_hi(addedInto.begin() + i, addedInto.end());

    // Add list to add.
    for (int j = 0; j < toAdd.size(); j++)
    {
        split_lo.push_back(toAdd[j]);
    }

    // Add rest of list.
    for (int j = 0; j < split_hi.size(); j++)
    {
        split_lo.push_back(split_hi[j]);
    }

    return split_lo;
}

string removeAllFirstChars(string input, string character)
{
    while (startsWith(input, character))
    {
        input = ltrim(input);
    }

    return input;
}

double convertVarToDouble(TVarObj var)
{
    double out;
    visit(
        overloaded{
            [&](TList) {},
            [&](bool) {},
            [&](int &var) {out = (double)var;},
            [&](float &var) {out = (double)var;},
            [&](string) {},
            [&](TPointer) {},
            [&](TIterator) {}
        },
        var
    );
    return out;
}

int convertIteratorToString(TVarObj iterator)
{
    int out;
    visit(
        overloaded{
            [&](TList) {},
            [&](bool) {},
            [&](int) {},
            [&](float) {},
            [&](string) {},
            [&](TPointer) {},
            [&](TIterator &var)
            {
                out = get<1>(var);
            }
        },
        iterator
    );

    return out;
}

float addObjs(TVarObj left, TVarObj right)
{
    float out;
    visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
        [&](int &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var + var2;},
                [&](int &var2) {out = (float)(var + var2);}}, right);},
        [&](float &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var + var2;},
                [&](int &var2) {out = (float)(var + var2);}}, right);}}, left);

    return out;
}

float subtractObjs(TVarObj left, TVarObj right)
{
    float out;
    visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
        [&](int &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var - var2;},
                [&](int &var2) {out = (float)(var - var2);}}, right);},
        [&](float &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var - var2;},
                [&](int &var2) {out = (float)(var - var2);}}, right);}}, left);

    return out;
}

float multiplyObjs(TVarObj left, TVarObj right)
{
    float out;
    visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
        [&](int &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var * var2;},
                [&](int &var2) {out = (float)(var * var2);}}, right);},
        [&](float &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var * var2;},
                [&](int &var2) {out = (float)(var * var2);}}, right);}}, left);

    return out;
}

float divideObjs(TVarObj left, TVarObj right)
{
    float out;
    visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
        [&](int &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var / var2;},
                [&](int &var2) {out = (float)(var / var2);}}, right);},
        [&](float &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = var / var2;},
                [&](int &var2) {out = (float)(var / var2);}}, right);}}, left);

    return out;
}

float exponentiateObjs(TVarObj left, TVarObj right)
{
    float out;
    visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
        [&](int &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = (float)pow(var, var2);},
                [&](int &var2) {out = (float)pow(var, var2);}}, right);},
        [&](float &var) {visit(overloaded{[&](TList) {},[&](string) {},[&](TPointer) {},[&](TIterator) {},[&](bool) {},
                [&](float &var2) {out = (float)pow(var, var2);},
                [&](int &var2) {out = (float)pow(var, var2);}}, right);}}, left);

    return out;
}