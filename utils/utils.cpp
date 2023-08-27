#include "../raylib/src/raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <map>
#include <tuple>

using namespace std;

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
    auto to_string = [](auto &&r) -> string
    {
        const auto data = &*r.begin();
        const auto size = static_cast<size_t>(std::ranges::distance(r));

        return string{data, size};
    };

    const auto range = toSplit |
                      std::ranges::views::split(character) |
                      std::ranges::views::transform(to_string);

    return {std::ranges::begin(range), std::ranges::end(range)};
}

bool startsWith(const string &str, const string &prefix) 
{
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
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