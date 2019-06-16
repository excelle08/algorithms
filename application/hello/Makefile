all: test-obj
	$(CC) -o test $(COMMON) test.o

test-obj:
	$(CC) $(CPPFLAGS) -fno-stack-protector -c test.c

print:
	$(info CPPFLAGS="$(CPPFLAGS)")
	$(info COMMON="$(COMMON)")

clean:
	rm -f *.o
	rm -f test
