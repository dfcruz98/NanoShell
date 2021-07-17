
# Compiler flags
CFLAGS=-Wall -Wextra -ggdb -std=c11 -pedantic -Wno-implicit-fallthrough -D_POSIX_C_SOURCE=200809L

# Name of the executable
PROGRAM=exec

all: $(PROGRAM)

main.o: main.c

# Object files required to build the executable
PROGRAM_OBJS=main.o

$(PROGRAM): $(PROGRAM_OBJS)
	$(CC) -o $@ $(PROGRAM_OBJS)

# entry to create the list of dependencies
depend:
	$(CC) -MM *.c

#how to create an object file (.o) from C file (.c)
.c.o:
	$(CC) $(CFLAGS) -c $<

# Remove compiled files 
clean:
	rm -f *.o core.* *~ $(PROGRAM) *.bak