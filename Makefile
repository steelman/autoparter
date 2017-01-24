LEX = flex
LFLAGS = -I
YACC = bison
YFLAGS = -d -o y.tab.c

SOURCES = autoparter.c \
	scanner.c \
	parser.c

include $(sources:.c=.d)

.PRECIOUS: parser.c scanner.c

%.h: %.y %.c
	mv y.tab.h $@ || true

scanner.o: parser.h

%.d: %.c
	$(CC) -M $(CPPFLAGS) $< > $@

autoparter: scanner.o parser.o autoparter.o

clean:
	rm -f $(SOURCES:.c=.d)
	rm -f *.o *.yy.c *.tab.[hc] *.d
