EXECUTABLE = psn_cli
OBJECTS = psn.o psn_cli.o 

CC 		= gcc
CFLAGS 	= -Wall -Wextra
IFLAGS  = -Imosquitto/lib -Iuthash/src
LDFLAGS = -Lmosquitto/lib -lmosquitto

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(IFLAGS) $(LDFLAGS) -o $@ $^
	
%.o: %.c 
	$(CC) $(CFLAGS) $(IFLAGS) -MMD -c $<

run: $(EXECUTABLE)
	@echo Running $<
	-./$< 
	@echo returned $$?

test: $(EXECUTABLE)
	echo test

clean:
	-rm $(EXECUTABLE)
	-rm $(OBJECTS)
	-rm *.d