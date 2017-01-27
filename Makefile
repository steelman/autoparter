LEX = flex
LFLAGS = -I
YACC = bison
YFLAGS = -d -o y.tab.c

SOURCES = autoparter.c \
	scanner.c \
	parser.c

autoparter: scanner.o parser.o autoparter.o

include $(SOURCES:.c=.d)

.PRECIOUS: parser.c scanner.c

%.h: %.y %.c
	mv y.tab.h $@ || true

%.d: %.c
	$(CC) -M $(CPPFLAGS) $< > $@

scanner.d: parser.h

clean:
	rm -f $(SOURCES:.c=.d)
	rm -f *.o *.yy.c *.tab.[hc] *.d
	rm -f scanner.c parser.[hc]
