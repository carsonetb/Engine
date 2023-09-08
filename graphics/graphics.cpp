// Graphics Handler 
// --------------------------------------------------

// Open, close, draw on, and interact with graphics
// windows.
// Header: graphics/graphics.h

#include "../utils/utils.h"
#include "../raylib/src/raylib.h"
#include "../parser/function.h"
#include "../parser/varcomp.h"
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

// Handle a call to the graphics system.
TVarObj handleGraphicsFunctionCall(string callLine, map<string, TVarObj> programVars, map<string, TIterator> iteratorsNameAccess)
{
    TFunctionCall funcCall = unpackFunctionCallLine(callLine);
    funcCall.name = splitString(funcCall.name, "graphics|")[1];

    if (funcCall.name == "open")
    {
        int width = convertVarToDouble(  getArgVal(funcCall.arguments[0], "int",    programVars, iteratorsNameAccess));
        int height = convertVarToDouble( getArgVal(funcCall.arguments[1], "int",    programVars, iteratorsNameAccess));
        string name = convertVarToString(getArgVal(funcCall.arguments[2], "string", programVars, iteratorsNameAccess), programVars, 
                                           iteratorsNameAccess);
        InitWindow(width, height, name.data());
    }

    if (funcCall.name == "targetFPS")
    {
        int fps = convertVarToDouble(getArgVal(funcCall.arguments[0], "int", programVars, iteratorsNameAccess));
        SetTargetFPS(fps);
    }

    if (funcCall.name == "shouldClose")
    {
        return WindowShouldClose();
    }

    if (funcCall.name == "close")
    {
        CloseWindow();
    }

    // Drawing objects

    if (funcCall.name == "drawFPS")
    {
        int x = convertVarToDouble(getArgVal(funcCall.arguments[0], "float", programVars, iteratorsNameAccess));
        int y = convertVarToDouble(getArgVal(funcCall.arguments[1], "float", programVars, iteratorsNameAccess));
        DrawFPS(x, y);
    }

    if (funcCall.name == "drawStart")
    {
        BeginDrawing();
    }

    if (funcCall.name == "drawEnd")
    {
        EndDrawing();
    }

    if (funcCall.name == "fill")
    {
        unsigned char r = convertVarToDouble(getArgVal(funcCall.arguments[0], "int", programVars, iteratorsNameAccess));
        unsigned char g = convertVarToDouble(getArgVal(funcCall.arguments[1], "int", programVars, iteratorsNameAccess));
        unsigned char b = convertVarToDouble(getArgVal(funcCall.arguments[2], "int", programVars, iteratorsNameAccess));
        unsigned char a = convertVarToDouble(getArgVal(funcCall.arguments[3], "int", programVars, iteratorsNameAccess));
        ClearBackground(Color{r, g, b, a});
    }

    if (funcCall.name == "drawRect")
    {
        int x =      convertVarToDouble(getArgVal(funcCall.arguments[0], "float", programVars, iteratorsNameAccess));
        int y =      convertVarToDouble(getArgVal(funcCall.arguments[1], "float", programVars, iteratorsNameAccess));
        int width =  convertVarToDouble(getArgVal(funcCall.arguments[2], "float", programVars, iteratorsNameAccess));
        int height = convertVarToDouble(getArgVal(funcCall.arguments[3], "float", programVars, iteratorsNameAccess));
        unsigned char r =      convertVarToDouble(getArgVal(funcCall.arguments[4], "int",   programVars, iteratorsNameAccess));
        unsigned char g =      convertVarToDouble(getArgVal(funcCall.arguments[5], "int",   programVars, iteratorsNameAccess));
        unsigned char b =      convertVarToDouble(getArgVal(funcCall.arguments[6], "int",   programVars, iteratorsNameAccess));
        unsigned char a =      convertVarToDouble(getArgVal(funcCall.arguments[7], "int",   programVars, iteratorsNameAccess));
        DrawRectangle(x, y, width, height, Color{r, g, b, a});
    }

    // Keys

    if (funcCall.name == "isKeyJustPressed")
    {
        int keyVal = convertVarToDouble(getArgVal(funcCall.arguments[0], "int", programVars, iteratorsNameAccess));
        return IsKeyPressed(keyVal);
    }

    if (funcCall.name == "isKeyDown")
    {
        int keyVal = convertVarToDouble(getArgVal(funcCall.arguments[0], "int", programVars, iteratorsNameAccess));
        return IsKeyDown(keyVal);
    }

    return 1;
}