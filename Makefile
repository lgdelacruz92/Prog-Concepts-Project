subc: subc.cpp parser.o
	g++ subc.cpp parser.o -o subc

parser_string_node_tests: ./tests/parser_string_node_tests.cpp parser.o
	g++ --std=c++17 ./tests/parser_string_node_tests.cpp parser.o -o parser_string_node_tests

parser_subprogs_tests: ./tests/parser_subprogs_tests.cpp parser.o
	g++ --std=c++17 ./tests/parser_subprogs_tests.cpp parser.o -o parser_subprogs_tests

parser_expression_tests: ./tests/parser_expression_tests.cpp parser.o
	g++ --std=c++17 ./tests/parser_expression_tests.cpp parser.o -o parser_expression_tests

parser_assignment_tests: ./tests/parser_assignment_tests.cpp parser.o
	g++ --std=c++17 ./tests/parser_assignment_tests.cpp parser.o -o parser_assignment_tests

parser_statement_tests: ./tests/parser_statement_tests.cpp parser.o
	g++ --std=c++17 ./tests/parser_statement_tests.cpp parser.o -o parser_statement_tests

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