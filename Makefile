EXECUTABLE = psn
OBJECTS = psn.o 

CC 		= gcc
CFLAGS 	= -Wall -Wextra
IFLAGS  = -I/home/gerrits/Work/im_proj/mosquitto-1.3.1/lib
LDFLAGS = -L/home/gerrits/Work/im_proj/mosquitto-1.3.1/lib -lmosquitto

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(IFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) $(IFLAGS) -MMD -c $<

clean:
	rm $(EXECUTABLE)
	rm $(OBJECTS)