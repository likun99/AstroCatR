# The user defined objects

BIN_DIR	?= ../bin
SOURCE_DIR ?= .
OBJS_DIR ?= ./objs
TARGET	?=	xingbiao1
OBJS	?=	\
	$(OBJS_DIR)/ang2pix_nest.c.o \
	$(OBJS_DIR)/HTM.c.o \
	$(OBJS_DIR)/mk_xy2pix.c.o \
	$(OBJS_DIR)/readfits.c.o \
	$(OBJS_DIR)/readtableconf.c.o \
	$(OBJS_DIR)/main.c.o \
	$(OBJS_DIR)/getimagetime.c.o 

	

LIBDIRFLAGS := \
	-L/usr/local/lib

LIBFLAGS	:=    -lm  -lcfitsio


# Compilers

CXX        := g++
CC         := gcc
LINK       := g++ -fPIC
# Includes
INCLUDES  += -I.   -I/usr/local/include

# Warning flags
CXXWARN_FLAGS := \
	-W -Wall \
	-Wimplicit \
	-Wswitch \
	-Wformat \
	-Wchar-subscripts \
	-Wparentheses \
	-Wmultichar \
	-Wtrigraphs \
	-Wpointer-arith \
	-Wcast-align \
	-Wreturn-type \
	-Wno-unused-function \
	$(SPACE)

CWARN_FLAGS := $(CXXWARN_FLAGS) \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wmissing-declarations \
	-Wnested-externs \
	-Wmain \


# Common flags
COMMONFLAGS += $(INCLUDES) -DUNIX
COMMONFLAGS += -g -O2 
CXXFLAGS    += -fno-strict-aliasing
CFLAGS      += -fno-strict-aliasing


All:	$(TARGET)

$(TARGET):	makeobjsdir $(OBJS) 
	$(LINK) -o $(TARGET) $(OBJS) $(CXXFLAGS) $(LIBDIRFLAGS) $(LIBFLAGS)
	cp $(TARGET) $(BIN_DIR)
	
makeobjsdir:
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.c.o : %.c 
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS_DIR)/%.cc.o : %.cc 
	$(CXX) $(CXXFLAGS) -o $@ -c $<



.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS_DIR)


