# ==============================================================================
# Variables
# ==============================================================================

CC      := g++
CFLAGS  := -g -std=c++0x -Wall -Winline -O3 
CFLAGS += -I../ 
CFLAGS += -I/home/wenbinx/transactional_memory/citrus
CFLAGS += -I/home/wenbinx/transactional_memory/vacation-experiment/tl2-x86-0.9.6
CFLAGS += -I./include 
#CFLAGS  += -m32
#CFLAGS  += -DTL2_OPTIM_HASHLOG
#CFLAGS  += -DTL2_RESIZE_HASHLOG
LD      := g++

LBSTM := libbstm.a

INCLUDE := include
SRCS := \
	$(INCLUDE)/abstract_lock.cpp \
	$(INCLUDE)/base_container.cpp \
	$(INCLUDE)/transaction.cpp 	\
	$(INCLUDE)/tx_exceptions.cpp  \
	boosted_map.cpp 	\
	boosted_list.cpp \
	boosted_citrus.cpp \
	boosted_skiplist.cpp \
	../debug.cpp \
	#
OBJS := ${SRCS:.cpp=.o}

AR      := ar
RANLIB  := ar s

RM := rm -f


# ==============================================================================
# Rules
# ==============================================================================

.PHONY: default
default: $(LBSTM)

.PHONY: clean
clean:
	$(RM) $(LBSTM) $(OBJS)

abstract_lock.cpp : rw_lock.h
	touch abstract_lock.cpp
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


$(LBSTM): $(OBJS)
	$(AR)  cru $@ $^
	$(RANLIB) $(LBSTM)
	$(RM) -rf $(OBJS)



# ==============================================================================
# Dependencies
# ==============================================================================

%.o: %.h


