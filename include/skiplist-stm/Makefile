# ==============================================================================
# Variables
# ==============================================================================

STM := /home/wenbinx/transactional_memory/vacation-experiment/tl2-x86-0.9.6
LIB := /home/wenbinx/transactional_memory/vacation-experiment/lib

CC      := gcc
CFLAGS  := -g -Wall -Winline -O0 -std=gnu99 -pthread -gdwarf-2
CFLAGS  += -I$(LIB)
CPPFLAGS := $(CFLAGS)
LDFLAGS  += -L$(STM) -L$(LIBS) -pthread
LIBS     += -ltl2

#CFLAGS  += -m32
#CFLAGS  += -DTL2_OPTIM_HASHLOG
#CFLAGS  += -DTL2_RESIZE_HASHLOG
LD      := gcc

TARGET := test-stm

CSRCS := \
	test-stm.c \
	sl-map.c \
	$(LIB)/list.c \
	$(LIB)/thread.c \
	$(LIB)/random.c \
	$(LIB)/mt19937ar.c \


OBJS := ${CSRCS:.c=.o}

RM := rm -f


# ==============================================================================
# Rules
# ==============================================================================

.PHONY: all

all: MAP_FLAG := -DMAP_USE_SKIPLIST
all: STM_FLAG := -DSTM -I$(STM) 
all: $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(TARGET)


.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(STM_FLAG) $(SET_FLAG) -c $< -o $@

# ==============================================================================
# Dependencies
# ==============================================================================

%.o: %.h


