# ==============================================================================
# Variables
# ==============================================================================

LIB := ../..
BSTM := $(LIB)/bstm
STM := ../../../../tl2-x86-0.9.6
CITRUS := /home/wenbinx/transactional_memory/citrus


CC      := g++
CFLAGS  := -g -std=c++0x -Wall -Winline -O0 
CFLAGS   += -I$(STM) -I$(BSTM)  -I$(LIB) -I$(CITRUS)
CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM) -L$(BSTM) -L$(CITRUS) -pthread
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
	$(LIB)/random.c \
	$(LIB)/mt19937ar.c \
	$(CITRUS)/citrus.c \
	$(CITRUS)/new_urcu.c \


OBJS := ${CPPSRCS:.cpp=.o}
OBJS += ${CSRCS:.c=.o}

RM := rm -f


# ==============================================================================
# Rules
# ==============================================================================

default: tl2

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


.PHONY: citrus
citrus: SET_FLAG := -DSET_USE_CITRUS
citrus: STM_FLAG := -DBSTM
citrus: $(OBJS)
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


