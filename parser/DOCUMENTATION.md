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
To install T, clone the repository using the command:

git clone https://github.com/carsonetb/Engine.git
cd Engine

Add submodules:

git submodule init
git submodule update

Now compile and run using make:

make
./main [filename]

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

Currently T's if statement has limitations. You
cannot use 'or' or 'and' operators (|| or &&) or 
seperate those with perenthasis. This functionality
will be added in the future.

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