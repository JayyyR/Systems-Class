LIBS = -lm

splot:	splot.o child.o
	gcc -o splot splot.o child.o $(LIBS)

child.o: child.h

splot.o: child.h

clean:
	rm -f core *.o splot
