
main : utils/utils.cpp graphics/graphics.cpp main.cpp parser/parser.cpp parser/listcomp.cpp parser/varcomp.cpp parser/ifcomp.cpp parser/function.cpp 
	g++ --std=c++20 -o $@ $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -g 