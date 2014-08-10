EXECUTABLE = psn_cli

TEST_EXE = psn_test

OBJECTS =   psn.o \
            psn_cli.o \
            init.o

OTHER_OBJECTS = jansson/src/lib/libjansson.a \
                mosquitto/lib/libmosquitto.a \
                libtomcrypt/libtomcrypt.a 

LIBFLAGS = -DUSE_GMP -DGMP_DESC

CC      =   gcc

CFLAGS 	=   -Wall -Wextra -std=c99 $(LIBFLAGS)


IFLAGS  =   -Imosquitto/lib \
            -Iuthash/src \
            -Ijansson/src \
            -Ilibtomcrypt/src/headers/


LDFLAGS =   -lssl \
            -lgmp \
            -lcares

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(OTHER_OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LDFLAGS)

jansson/src/lib/libjansson.a:
	-cd jansson/src/; cmake ..; make; cd ../../

mosquitto/lib/libmosquitto.a:
	-cd mosquitto; make; cd ..

libtomcrypt/libtomcrypt.a:
	-cd libtomcrypt; CFLAGS="$(LIBFLAGS)" make; cd ..

%.o: %.c 
	$(CC) $^ $(CFLAGS) $(IFLAGS) -MMD -c $< 

run: $(EXECUTABLE)
	@echo Running $<
	-./$< 
	@echo returned $$?

test: $(TEST_EXE)
	@echo test

stats: $(OBJECTS)
	@cloc --exclude-dir=uthash,mosquitto,libtomcrypt,jansson,config,docs,.git --exclude-lang=D,Javascript .

clean: 
	-rm $(EXECUTABLE)
	-rm $(OBJECTS)
	-rm *.d
