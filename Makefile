subc: subc.cpp parser.o
	g++ subc.cpp parser.o -o subc

subc_tests: ./tests/subc_tests.cpp parser.o
	g++ ./tests/subc_tests.cpp parser.o -o subc_tests

parser_body_tests: ./tests/parser_body_tests.cpp parser.o
	g++ --std=c++17 ./tests/parser_body_tests.cpp parser.o -o parser_body_tests

parser_identifer_tests: ./tests/parser_identifer_tests.cpp parser.o
	g++ --std=c++11 ./tests/parser_identifer_tests.cpp parser.o -o parser_identifer_tests

parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o


clean: a.out subc
	rm a.out subc