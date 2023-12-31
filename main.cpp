#include "raylib/src/raylib.h"
#include "parser/parser.h"
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    int exitCode;
    if (argc > 1)
    {
        exitCode = parseRun(argv[1]);
    }
    else 
    {
        exitCode = 2;
        cout << "Err: No filename passed." << endl;
    }
    
    if (exitCode == 3)
    {
        cout << "Program exited with exit code 3: Expected fatal error." << endl;
    }
    if (exitCode == 2)
    {
        cout << "Program exited with exit code 2: Incomplete." << endl;
    }
    if (exitCode == 1)
    {
        cout << "Program exited with exit code 1: Fatal error." << endl;
    }
    if (exitCode == 0)
    {
        cout << "Program excited with exit code 0." << endl;
    }

    return 0;
}