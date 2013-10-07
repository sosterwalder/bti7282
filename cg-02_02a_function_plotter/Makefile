PROG=funcplotter
PROGDEBUG=funcplotterd
OBJS=funcplotter.cppo UtilGlut.cppo zpr.co
DEBUGOBJS=funcplotter.cpp UtilGlut.cpp zpr.c

CC=gcc
CFLAGS=-Wall -pedantic
LD=gcc
LFLAGS=-lstdc++ -lm -lGL -lGLU -lglut -I. -L.
LDEBUGFLAGS=-lstdc++ -lm -lGL -lGLU -lglut
DEBUGFLAGS=-DDEBUG -g
RM=rm -f

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

%.cppo: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

%.co: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

debug: 
	$(LD) $(DEBUGFLAGS) $(LDEBUGFLAGS) $(DEBUGOBJS) -o $(PROGDEBUG)

clean:
	$(RM) $(PROG) $(OBJS) $(PROGDEBUG)

nowin:
	$(RM) $(PROG).cpp.bak $(PROG).exe $(PROG).ncb $(PROG).sln $(PROG).suo $(PROG).vcproj $(PROG)-*.sln $(PROG)-*.vcproj $(PROG)-*.vcxproj

run: all
	./$(PROG)
