# Overview
T is a custom language interpretter created by Carson 
Bates for this engine. T stands for turtle, because 
it's likely very slow. It can also be compiled by 
converting it to c++ (a very crude workaround to not
wanting to ever have to build a compiler) and then 
compiling that. This file is documentation for 
everything you can do in the language.

=====================================================

# Installation
You'll need to install g++ before doing any of this.
You can install it at 'https://gcc.gnu.org/'.
To install T, clone the repository using the command:

git clone https://github.com/carsonetb/Engine.git
cd Engine

Add submodules:

git submodule init
git submodule update

Now compile and run using make:

make
./main [filename]

NOTE: On windows, you don't need the ./ before 
'main'.
An example program is located at 
examples/testScript.txt, so type the command ...

./main examples/testScript

... to run it.

# Keywords
Keywords, like var, for, or if are the backbones of a
programming language.

## Keyword: If
If is a simple but powerful keyword to check if 
something is true or false. In T, it is used like
this:

if [type] [obj] [operator] [type] [obj] {
    ...
}

For example:

if int 5 != int 7 {
    ... (true)
}

You can also use variables in if statements:

if var [variable-name] [operator] var [variable-name] {
    ...
}

Or compare variables with hardcoded objects:

if var [variable-name] [operator] [type] [obj] {
    ...
}

For example:

var string a = 5
if var a == int 5 {
    ... (true)
}

You can seperate checks with 'or' and 'and' operators
('&&' and '||'). For example:

var int a = 5
var int b = 7

if var a < var b && var a == int 5 || string gobbledeegook == string something else {
    ... (true)
}

T currently does not support using parenthasis in if
statements, although it will be added in the future.

## Keyword: var (utils/TVarObj)
T variables are simple and similar to most other
languages. Simply type the variable's name, the type,
the name, and the value of the variable:

var [type] [name] = [value]

### Variable Type: String (std::string)
Strings are simple, they hold a string of characters.
The value of the variable should be the value of the
string:

var string [name] = this is a string

You can then access you variable anywhere else were
it is appropriate.

### Variable Type: Bool
Bools can declared like strings. They are either true
or false:

var bool [name] = true

### Variable Type: Int/Float 
Integers and floating point numbers hold any rational
number. Special operators can be used with this 
variable type in if statements: <, >, <=, and >=. 
These compare the size of the variable to another.

Variable creation:

var int a = 5
var float b = 7.46

Special operators:

if a < b {
    ... (true)
}

### Variable Type: Pointer (utils/TPointer)
A pointer is a variable that points to another 
variable. If it is changed it also changes the 
variable it's pointing to. Currently you cannot 
create a pointer type.

### Variable Type: Iterator (utils/TIterator)
An iterator holds a number, a start value, and a stop
value. It can be used as an integer, but can't be 
created by the user. The start and stop value can
only be accessed by the interpreter.

### Variable Type: List (utils/TList)
A list is a fairly complex variable type. It can hold
multiple of a type, but only one type. It can also
hold another list, which can hold a type. You can
access any element in this list, or an element in
a list inside of the list. You can create one like
so:

var list:int a = [1, 2, 3]

You can access a list also:

coutstreamadd [1]a

Which prints '2'.
You can also create a list of lists:

var list:list:int b = [[5, 4], [6, 7]]

Printing an element in that list:

coutstreamadd [0][1]b

Which prints '6'.
Subsequently if we changed it to this:

coustreamadd [0][0]b 

It would print '5'.

## Keyword: for
T's for loops are like most other programming 
languages: create iterator, which loops from start
to stop:

for i 0 10 {
    ...
}

In this loop, the 'i' variable starts at zero and 
iterates to 10.
You can also create infinite loops.

for i 2 : {
    ...
}

The colon signifies that there is no end to the loop.
'i' now starts at two and will keep increasing until
the 'break' keyword is called.

## Keyword: break
'break' will exit the currently running for loop. It 
will not wait to complete any task.

## Keyword: quit
'quit' will exit the program emmediately, as if it
was finished, not like an error.

# Errors
Errors are created when the program runs into a 
problem where it is impossible to continue (variable
is created that isn't the type specified, invalid 
syntax, etc.). These are errors that can happen when
running your program.

## Exit Codes
Exit codes are general ways the program can exit.

### Exit Code: 0
This exit code is good. It means your program exited
successfully either by the interpretter reaching the
last line or by a call of 'quit'.

### Exit Code: 1
This exit code means an error occured, and the 
program stopped before it could reach the end. This 
most likely means there is a bug in your program.

### Exit Code: 2
This means the program never got a chance to run.
Usually this means no file name was passed as a
keyword argument when running the interpretter.

### Exit Code: 3
This is an error that was expected. This is usually
an error that is supposed to happen, for example by
a call of 'throwError'. This isn't neccesarily good
or bad.

## Interpretter Errors

### SyntaxError
This is probably the most common error. It means you
made a typo somehow.

### AttributeError
This means that an objects attribute is incorrect.
For example, a pointer which pointed to void should
throw this error.

### IOError
This error currently only can happen if the 
interpretter fails to open the file. Check if your
file name is correct!

# Warnings
A warning is a message warning you of something that
could cause an error in the future (of writing your
code, or executing the program). A warning could be
expected, or it could help you find the cause of a
fatal flaw in your code. There are no specific types
of warnings, but they aim to be as detailed as 
possible.