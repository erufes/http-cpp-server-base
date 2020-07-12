CC			    = g++
RM			    = rm -f
CPPFLAGS		= -Wall -std=c++11 -g #-DDEBUG -DKEY_AND_MOUSE_EVENT_VERBOSE

LDDFLAGS		= -lglut -lGL -lX11 -lGLU -lm -ljsoncpp -pthread
SRCEXT := cpp
SOURCESDIR = src
OBJDIR := obj
BUILDDIR := bin
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

TARGET := bin/erushttpserver

all: ${TARGET}

clean:
	-rm -r $(OBJDIR)

cleanall: clean
	-rm -r $(BUILDDIR)

%.o: %.cpp
	$(CC) -c $< $(CPPFLAGS)
	cd ..
	$(shell mkdir -p $(OBJDIR))
	mv *.o ./$(OBJDIR)
	cd $(OBJDIR)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(shell mkdir -p $(BUILDDIR))
	@echo " $(CC) $(shell find $(OBJDIR) -type f -name *.o) -o $(TARGET) $(CPPFLAGS)"; $(CC) $(shell find $(OBJDIR) -type f -name *.o) -o $(TARGET) $(CPPFLAGS) $(LDDFLAGS)

