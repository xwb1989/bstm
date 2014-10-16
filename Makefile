# ==============================================================================
# Variables
# ==============================================================================

CC      := g++
CFLAGS  := -g -std=c++0x -Wall -Winline -O0 -I../
#CFLAGS  += -m32
#CFLAGS  += -DTL2_OPTIM_HASHLOG
#CFLAGS  += -DTL2_RESIZE_HASHLOG
LD      := g++

LBSTM := libbstm.a

SRCS := \
	abstract_lock.cpp \
	base_container.cpp \
	boosted_map.cpp 	\
	transaction.cpp 	\
	tx_exceptions.cpp  \
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


