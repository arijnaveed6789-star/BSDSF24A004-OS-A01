CC = gcc
CFLAGS = -Wall -I../include
AR = ar
ARFLAGS = rcs

OBJDIR = ../obj
BINDIR = ../bin
LIBDIR = ../lib

STATIC_LIB = $(LIBDIR)/libmyutils.a
DYNAMIC_LIB = $(LIBDIR)/libmyutils.so

TARGET_STATIC = $(BINDIR)/client_static
TARGET_DYNAMIC = $(BINDIR)/client_dynamic

LIB_SOURCES = mystrfunctions.c myfilefunctions.c
LIB_OBJECTS = $(LIB_SOURCES:%.c=$(OBJDIR)/%.o)
PIC_OBJECTS = $(LIB_SOURCES:%.c=$(OBJDIR)/%_pic.o)

MAIN_OBJ = $(OBJDIR)/main.o

.PHONY: all static dynamic clean

all: static dynamic

static: $(TARGET_STATIC)
dynamic: $(TARGET_DYNAMIC)

$(STATIC_LIB): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $(STATIC_LIB) $(LIB_OBJECTS)

$(TARGET_STATIC): $(MAIN_OBJ) $(STATIC_LIB)
	$(CC) $(MAIN_OBJ) -L$(LIBDIR) -lmyutils -o $(TARGET_STATIC)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%_pic.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(DYNAMIC_LIB): $(PIC_OBJECTS)
	$(CC) -shared -o $(DYNAMIC_LIB) $(PIC_OBJECTS)

$(TARGET_DYNAMIC): $(MAIN_OBJ) $(DYNAMIC_LIB)
	$(CC) $(MAIN_OBJ) -L$(LIBDIR) -l:libmyutils.so -o $(TARGET_DYNAMIC)

clean:
	rm -f $(OBJDIR)/*.o $(STATIC_LIB) $(DYNAMIC_LIB) $(TARGET_STATIC) $(TARGET_DYNAMIC)
