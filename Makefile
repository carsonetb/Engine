
main : main.cpp parser/parser.cpp utils/utils.cpp parser/listcomp.cpp parser/varcomp.cpp
	g++ --std=c++20 -o $@ $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -g