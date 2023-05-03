run-test: test
	./test

test: long_int.cpp test.cpp
	g++ -std=c++11 -Werror -o test test.cpp long_int.cpp


main: long_int.cpp  main.cpp
	g++ -std=c++11 -Werror -o main long_int.cpp main.cpp