subc: subc.cpp parser.o
	g++ subc.cpp parser.o -o subc

parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o

clean: a.out subc
	rm a.out subc