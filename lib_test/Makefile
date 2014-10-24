# ==============================================================================
# Variables
# ==============================================================================

LIB := ../..
BSTM := $(LIB)/bstm
STM := ../../../../tl2-x86-0.9.6


CC      := g++
CFLAGS  := -g -std=c++0x -Wall -Winline -O3 
CFLAGS   += -I$(STM) -I$(BSTM) -I$(LIB)
CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM) -L$(BSTM) -pthread
LIBS     += -ltl2 -lbstm -ltbb
#CFLAGS  += -m32
#CFLAGS  += -DTL2_OPTIM_HASHLOG
#CFLAGS  += -DTL2_RESIZE_HASHLOG
LD      := g++

TARGET := work_load

CPPSRCS := \
	work_load.cpp \
	
CSRCS := \
	$(LIB)/list.c \
	$(LIB)/thread.c \


OBJS := ${CPPSRCS:.cpp=.o}
OBJS += ${CSRCS:.c=.o}

RM := rm -f


# ==============================================================================
# Rules
# ==============================================================================

default: boost

.PHONY: boost
boost: SET_FLAG := -DSET_USE_BOOST
boost: STM_FLAG := -DBSTM
boost: $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(TARGET)

.PHONY: tl2
tl2: SET_FLAG := -DSET_USE_TL2
tl2: STM_FLAG := -DSTM
tl2: $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(TARGET)



.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(STM_FLAG) $(SET_FLAG) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(STM_FLAG) $(SET_FLAG) -c $< -o $@

# ==============================================================================
# Dependencies
# ==============================================================================

%.o: %.h


