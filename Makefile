main: main.c elfheader.o elfsectionheader.o elfsection.o elfsymtable.o filereader.o elfrelocation.o
	gcc -o main main.c elfheader.o elfsectionheader.o elfsection.o elfsymtable.o filereader.o elfrelocation.o


elfheader.o: elfheader.c elfheader.h filereader.h
	gcc -c elfheader.c filereader.h

elfsectionheader.o: elfsectionheader.c elfsectionheader.h filereader.h
	gcc -c elfsectionheader.c filereader.h

elfsection.o: elfsection.c elfsection.h filereader.h
	gcc -c elfsection.c filereader.h

elfsymtable.o: elfsymtable.c elfsymtable.h filereader.h
	gcc -c elfsymtable.c filereader.h

elfrelocation.o: elfrelocation.c elfrelocation.h filereader.h
	gcc -c elfrelocation.c filereader.h

filereader.o: filereader.c filereader.h
	gcc -c filereader.c

clean:
	rm -f main elfheader.o elfsection.o elfsectionheader.o elfsymtable.o filereader.o elfrelocation.o
