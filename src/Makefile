CC = gcc
CFLAGS = -Wall -I../include
AR = ar
ARFLAGS = rcs

OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib

LIB_NAME = $(LIBDIR)/libmyutils.a
TARGET = $(BINDIR)/client_static

LIB_SOURCES = mystrfunctions.c myfilefunctions.c
LIB_OBJECTS = $(LIB_SOURCES:%.c=$(OBJDIR)/%.o)

MAIN_OBJ = $(OBJDIR)/main.o

.PHONY: all clean

all: $(TARGET)

$(LIB_NAME): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJECTS)

$(TARGET): $(MAIN_OBJ) $(LIB_NAME)
	$(CC) $(MAIN_OBJ) -L$(LIBDIR) -lmyutils -o $(TARGET)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(LIB_NAME) $(TARGET)
