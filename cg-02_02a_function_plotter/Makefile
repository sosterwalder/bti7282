platform=$(shell uname -s)
PROG=funcplotter
PROGDEBUG=funcplotterd
OBJS=funcplotter.cppo UtilGlut.cppo zpr.co
DEBUGOBJS=funcplotter.cpp UtilGlut.cpp zpr.c

CC=gcc
CFLAGS=-Wall -pedantic
LD=gcc

ifeq ($(platform),Darwin)
LFLAGS=-framework OpenGL -lstdc++ -lm -lGLUT -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib
LDEBUGFLAGS=-framework OpenGL -lstdc++ -lm -lGLUT -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib
else
LFLAGS=-lstdc++ -lm -lGL -lGLU -lglut -I. -L.
LDEBUGFLAGS=-lstdc++ -lm -lGL -lGLU -lglut
endif
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
