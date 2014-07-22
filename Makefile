EXECUTABLE = psn_cli

OBJECTS = psn.o psn_cli.o

OTHER_OBJECTS = jansson/src/lib/libjansson.a \
				mosquitto/lib/libmosquitto.a \
				libtomcrypt/libtomcrypt.a

CC 		= 	gcc
CFLAGS 	= 	-Wall -Wextra 
IFLAGS  = 	-Imosquitto/lib \
			-Iuthash/src \
			-Ijansson/src \
			-Ilibtomcrypt/src/headers/

LDFLAGS = 	-lssl \
			-lcares

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(OTHER_OBJECTS) $(LDFLAGS)
	
%.o: %.c 
	$(CC) $^	(CFLAGS) $(IFLAGS) -MMD -c $<

run: $(EXECUTABLE)
	@echo Running $<
	-./$< 
	@echo returned $$?

test: $(EXECUTABLE)
	@echo test

stats: $(OBJECTS)
	@cloc --exclude-dir=uthash,mosquitto,libtomcrypt,jansson --exclude-lang=D .

clean:
	-rm $(EXECUTABLE)
	-rm $(OBJECTS)
	-rm *.d