var list:list:string testList = [[hi, hello, ho], [hii, helloo, hoho]]
var int access = 0
print var [var:access][0]testList
print endl

var list:list:int testList2 = [[1, 5, 3], [7, 4, 3]]
var int access2 = 1
print var [var:access2][var:access]testList2
print endl

if var [var:access2][var:access]testList2 == 5 {
    print hellooo
    print endl
}

var int a = 5
varchange a + int 1
print var a 
print endl

print would you like to start? (y/n) 
input userComfirm

for i 0 10 {
    print looping 
    print var i 
    print endl 

    if it i > int 5 {
        print greater 
        print endl
        continue;
    }

    print less
    print endl
}

if var userComfirm == string n {
    quit
}
else {
    for i 0 : {
        print Which way would you like to go? (left/right/up/down) 
        input direction

        var int checkInt = 5
        if var direction == string right && var checkInt == int 5 || int 5 = int 7 {
            print You win!
            print endl 
            quit 
        }
        else {
            print Incorrect! Guess again.
            print endl
        }
    }
}