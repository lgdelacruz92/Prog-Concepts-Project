subc: subc.cpp parser.o
	g++ subc.cpp parser.o -o subc

subc_tests: ./tests/subc_tests.cpp parser.o
	g++ ./tests/subc_tests.cpp parser.o -o subc_tests

parser_identifier_tests: ./tests/parser_identifier_tests.cpp parser.o
	g++ --std=c++11 ./tests/parser_identifier_tests.cpp parser.o -o parser_identifier_tests

parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o


clean: a.out subc
	rm a.out subc