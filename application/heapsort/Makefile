DEP := $(BASE)/heap/heap.o $(COMMON)

all: test-obj demo-obj
	$(CC) -o test $(DEP) test.o
	$(CC) -o demo $(DEP) demo.o

test-obj:
	$(CC) $(CPPFLAGS) -c test.c

demo-obj:
	$(CC) $(CPPFLAGS) -c demo.c

print:
	$(info CPPFLAGS="$(CPPFLAGS)")
	$(info DEP="$(DEP)")

clean:
	rm -f *.o
	rm -f test
	rm -f demo
