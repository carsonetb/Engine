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
#include <variant>

using namespace std;

vector<string> unpackList(string input)
{
    input = trimChar(input, ' ');
    input = trimChar(input, ' ');
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    input.erase(input.begin());
    input.erase(input.end() - 1);
    string objString = "";
    vector<string> unpacked;
    bool ignoreCommas = false;

    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == '[')
        {
            objString.push_back(input[i]);
            ignoreCommas = true;
        }
        else if (input[i] == ']')
        {
            objString.push_back(input[i]);
            ignoreCommas = false;
        }
        else if (input[i] == ',' && !ignoreCommas)
        {
            unpacked.push_back(objString);
            objString = "";
            continue;
        }
        else 
        {
            objString.push_back(input[i]);
        }
    }

    unpacked.push_back(objString);

    return unpacked;
}
