driver: driver.o editor.o parser.o
	g++ -o editor driver.o editor.o parser.o

driver.o: driver.cpp editor.o parser.o
	g++ -c driver.cpp

editor.o: editor.cpp editor.h
	g++ -c editor.cpp

parser.o: parser.cpp parser.h
	g++ -c parser.cpp

clean:
	rm -rf *.o driver