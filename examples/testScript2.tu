# This script should be used to actually
# test the game engine.
using examples/testScriptExt.tu

callfunc graphics|open(1000, 800, test name)
var int x = 100
var int y = 100
var int speed = 1

for i 0 : {
    callfunc graphics|drawStart()
    callfunc graphics|fill(255, 255, 255, 255)
    callfunc graphics|drawFPS(10, 10)

    callfunc graphics|shouldClose() -> shouldClose

    if var shouldClose == bool true {
        break
    }

    callfunc graphics|isKeyDown(var KEY_A) -> left 
    callfunc graphics|isKeyDown(var KEY_S) -> down
    callfunc graphics|isKeyDown(var KEY_D) -> right 
    callfunc graphics|isKeyDown(var KEY_W) -> up 

    if var left == bool true {
        varchange x - var speed
    } 
    if var right == bool true {
        varchange x + var speed
    }
    if var up == bool true {
        varchange y - var speed
    }
    if var down == bool true {
        varchange y + var speed
    }

    callfunc graphics|drawRect(var x, var y, 100, 100, 0, 0, 0, 255)
    callfunc graphics|drawEnd()
}

callfunc graphics|close()