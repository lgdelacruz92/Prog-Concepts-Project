subc: subc.cpp
	g++ subc.cpp -o subc

clean: a.out subc
	rm a.out subc