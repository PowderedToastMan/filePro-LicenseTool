CC=cc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=licensetool
INSTALLPATH=/usr/local/bin

all : $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

install:
	cp $(EXECUTABLE) $(INSTALLPATH)
	strip $(INSTALLPATH)/$(EXECUTABLE)

uninstall:
	rm -f $(INSTALLPATH)/$(EXECUTABLE)

clean :
	rm -f $(OBJECTS) $(EXECUTABLE)
