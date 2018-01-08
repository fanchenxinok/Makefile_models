XX = g++
CC = gcc
CFLAGS = -g -Wall -fPIC

CLIBS = -L./lib -lSendMsgModel -lpthread -lrt -lprotobuf

INCLUDES = $(wildcard ./include/*.h)
INCLUDE_DIRS = -I./include -I./myListeners -I./myResponsers -I./logout -I./myMsgDef

XX_SOURCES = $(wildcard ./*.cpp ./myListeners/*.cpp ./myResponsers/*.cpp)
CC_SOURCES = $(wildcard ./logout/*.c)

XX_OBJECTS = $(patsubst %.cpp,%.o,$(XX_SOURCES))
CC_OBJECTS = $(patsubst %.c,%.o,$(CC_SOURCES))

# 用到protocol buffer
MSG_SUBDIR = ./myMsgDef
MSG_DEF_O = $(MSG_SUBDIR)/msgDef.pb.o

TARGET = test

$(TARGET) : $(XX_OBJECTS) $(CC_OBJECTS) $(MSG_DEF_O)
	$(XX) $^ -o $@ $(CLIBS)
	
$(XX_OBJECTS) : %.o : %.cpp
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

$(CC_OBJECTS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

export XX CFLAGS AR ARFLAG

# 用到protocol buffer
$(MSG_DEF_O) : $(MSG_SUBDIR)
$(MSG_SUBDIR):ECHO
	+$(MAKE) -C $@

ECHO:
	@echo $(SUBDIRS)
	@echo begin compile

.PHONY : clean
clean:
	for dir in $(MSG_SUBDIR);\
	do $(MAKE) -C $$dir clean||exit 1;\
	done
	rm -rf $(TARGET) $(XX_OBJECTS) $(CC_OBJECTS)
