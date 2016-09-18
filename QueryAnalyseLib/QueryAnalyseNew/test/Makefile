CC = g++

#CFLAGS = -lcurl -lquerylib
LOCAL_LIB_PATH = ./lib
LOCAL_INC_PATH = ./include
DEPLOY_PATH=./output
CPPFLAGS = -g -Wall -fPIC -D_REENTRANT -DLINUX
DEP_INC=./curllib/curl-7.46
AR = ar rcv

CSRC = $(wildcard *.c)
COBJS = $(patsubst %.c,%.o,$(CSRC))
SOURCES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SOURCES))
TARGET = $(LOCAL_LIB_PATH)/libqueryanalyse.a

.cpp.o:
	$(CC) $(CPPFLAGS) -I$(DEP_INC) -c $< -o $@
.c.o:
	$(CC) $(CPPFLAGS) -I$(DEP_INC) -c $< -o $@

all : mkdir $(TARGET)
mkdir : 
	@mkdir -p $(LOCAL_LIB_PATH)
	@mkdir -p output/{lib,include}

$(TARGET) : $(OBJS) $(COBJS)
	$(AR) $(@) $^
	cp *.h output/include
	cp lib/*.a output/lib
clean :
	rm -f  *.o $(LOCAL_LIB_PATH)/*.a
	rm -rf lib output
#main: $(objects)
#        $(CC) -o run  main.cpp -L. $(CFLAGS) -I.

