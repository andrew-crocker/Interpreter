ace: driver.o lexer.o parser.o editor.o
	g++ -o ace driver.o lexer.o parser.o editor.o

driver.o: driver.cpp lexer.o parser.o editor.o
	g++ -c driver.cpp

editor.o: editor.cpp editor.h
	g++ -c editor.cpp

lexer.o: lexer.cpp lexer.h parser.o
	g++ -c lexer.cpp

parser.o: parser.cpp parser.h
	g++ -c parser.cpp

clean:
	rm -rf *.o driver