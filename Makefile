autoparter.yy.c: autoparter.l
	flex autoparter.l

autoparter.tab.c: autoparter.y
	bison autoparter.y

autoparter.tab.h: autoparter.y
	bison autoparter.y

autoparter.yy.o: autoparter.yy.c autoparter.tab.h
autoparter.tab.o: autoparter.tab.c

autoparter: autoparter.tab.o autoparter.yy.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *.yy.c *.tab.[hc]
