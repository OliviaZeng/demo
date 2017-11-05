#gcc version

ROOT=$(LocaleGameBuileRoot)
include $(ROOT)/makefile.pub

CC  = gcc
CXX = g++
GCCVER := $(shell $(CC) -dumpversion | awk -F. '{ print $$1"."$$2}' )

CORE_INC=../include
CORE_SRC=../src/core
COMPILE_LIB_HOME=../lib
LUADIR = /usr/local/include

LIB_INC = -I$(ROOT)/pLib/sms_alarm -I$(ROOT)/pLib/Json -I$(ROOT)/pLib/udplog -I$(LUADIR) -I$(ROOT)/lua-intf-master -I$(ROOT)/pLib/curl/include
INC     = $(LIB_INC) $(PUB_INCLUDE) -I. 
OPT     = -pipe -fno-ident  
LINK    = -L/home/JasonZhu/ProtobufRpc/pLib/protobuf -lprotobuf -lnetproto 

CFLAGS += $(OPT) -gdwarf-2 -Wall -Werror  -fpermissive  -Wno-literal-suffix 
ifeq ($(MEMCHECK),1)
CFLAGS += -DMEMCHECK
endif
CXXFLAGS := $(CFLAGS)

SRCS += $(wildcard ./*.cpp ./*.c ./*.cc)

OBJS = $(patsubst %.cpp,%.o,$(patsubst %.c ,%.o ,$(patsubst %.cc,%.o, $(SRCS) ) ) )
OBJS_DEL = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o ,$(patsubst %.cc,%.o, $(SRCS) ) ) )

BIN_NAME = test 

.PHONY : all

all: $(BIN_NAME)

$(BIN_NAME): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LINK)

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
%.o: %.cc Makefile
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
clean:
	rm -rf $(OBJS_DEL) $(BIN_NAME)
install:
	mv ./$(BIN_NAME) ../bin/
