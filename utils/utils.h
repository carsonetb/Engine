#pragma once
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);
std::string trimChar(std::string s, char toTrim);
std::string trimString(std::string toTrim, std::string trimBy);
std::string readFile(std::string fileName);
std::vector<std::string> splitString(std::string toSplit, std::string character);
bool startsWith(const std::string &str, const std::string &prefix);
bool endsWith(const std::string &str, const std::string &suffix);
std::vector<std::string> insertVectorAtIndString(int i, std::vector<std::string> addedInto, std::vector<std::string> toAdd);
std::string removeAllFirstChars(std::string input, std::string character);
int si(std::string i);
int sf(std::string i);
typedef std::tuple<std::string, std::string> TPointer;
typedef std::tuple<std::string, int, int, int> TIterator;

struct TFunction 
{
    int createdLine;
    std::string returnType;
    std::string name;

    // Vector of pointers.
    std::vector<TPointer> arguments;
};

struct TFunctionCall
{
    std::string returnVarName;
    std::string name;

    // Vector of tuple of boolian to represent if it's a variable
    // and string for the value passed or a variable name.
    std::vector<std::tuple<bool, std::string>> arguments;
};

static inline bool operator<(const TIterator &lhs, const int &rhs) { return get<1>(lhs) < rhs; }
static inline bool operator>(const TIterator &lhs, const int &rhs) { return get<1>(lhs) > rhs; }
static inline bool operator<=(const TIterator &lhs, const int &rhs) { return get<1>(lhs) <= rhs; }
static inline bool operator>=(const TIterator &lhs, const int &rhs) { return get<1>(lhs) >= rhs; }
static inline bool operator==(const TIterator &lhs, const int &rhs) { return get<1>(lhs) == rhs; }
static inline bool operator!=(const TIterator &lhs, const int &rhs) { return get<1>(lhs) != rhs; }

static inline bool operator<(const TIterator &lhs, const float &rhs) { return get<1>(lhs) < rhs; }
static inline bool operator>(const TIterator &lhs, const float &rhs) { return get<1>(lhs) > rhs; }
static inline bool operator<=(const TIterator &lhs, const float &rhs) { return get<1>(lhs) <= rhs; }
static inline bool operator>=(const TIterator &lhs, const float &rhs) { return get<1>(lhs) >= rhs; }
static inline bool operator==(const TIterator &lhs, const float &rhs) { return get<1>(lhs) == rhs; }
static inline bool operator!=(const TIterator &lhs, const float &rhs) { return get<1>(lhs) != rhs; }

static inline bool operator<(const TIterator &lhs, const TIterator &rhs) { return get<1>(lhs) < get<1>(rhs); }
static inline bool operator>(const TIterator &lhs, const TIterator &rhs) { return get<1>(lhs) > get<1>(rhs); }
static inline bool operator<=(const TIterator &lhs, const TIterator &rhs) { return get<1>(lhs) <= get<1>(rhs); }
static inline bool operator>=(const TIterator &lhs, const TIterator &rhs) { return get<1>(lhs) >= get<1>(rhs); }
static inline bool operator==(const TIterator &lhs, const TIterator &rhs) { return get<1>(lhs) == get<1>(rhs); }
static inline bool operator!=(const TIterator &lhs, const TIterator &rhs) { return get<1>(lhs) != get<1>(rhs); }

typedef std::variant<bool, int, float, std::string, TPointer, TIterator> TListItem;

struct TList
{
    std::string itemType;
    std::vector<std::variant<TListItem, TList>> item;
    boost::uuids::uuid id = boost::uuids::random_generator()();
};

static inline bool operator==(const TList &lhs, const TList &rhs)
{
    return lhs.id == rhs.id;
}

static inline bool operator!=(const TList &lhs, const TList &rhs)
{
    return !(lhs == rhs);
}

static inline bool operator<(const TList &lhs, const TList &rhs) { return false; }
static inline bool operator>(const TList &lhs, const TList &rhs) { return false; }
static inline bool operator<=(const TList &lhs, const TList &rhs) { return false; }
static inline bool operator>=(const TList &lhs, const TList &rhs) { return false; }

static inline bool operator+=(const TList &lhs, const TList &rhs) { return false; }
static inline bool operator-=(const TList &lhs, const TList &rhs) { return false; }
static inline bool operator*=(const TList &lhs, const TList &rhs) { return false; }
static inline bool operator/=(const TList &lhs, const TList &rhs) { return false; }

typedef std::variant<bool, int, float, std::string, TPointer, TIterator, TList> TVarObj;
const std::map<int, std::string> TVarObjTypes = {{0, "bool"}, {1, "int"}, {2, "float"}, {3, "string"}, {4, "pointer"}, {5, "iterator"}, {6, "list"}};
typedef int TError;

double convertVarToDouble(TVarObj var);
int convertIteratorToString(TVarObj iterator);
float addObjs(TVarObj left, TVarObj right);
float subtractObjs(TVarObj left, TVarObj right);
float multiplyObjs(TVarObj left, TVarObj right);
float divideObjs(TVarObj left, TVarObj right);
float exponentiateObjs(TVarObj left, TVarObj right);