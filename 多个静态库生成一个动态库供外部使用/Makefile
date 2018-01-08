XX = g++
CC = gcc
CFLAGS = -g -Wall -fPIC

CLIBS = -L./lib -lSendMsgModel -lpthread -lrt 

INCLUDES = $(wildcard ./include/*.h)
INCLUDE_DIRS = -I./include -I./myListeners -I./myResponsers -I./logout

XX_SOURCES = $(wildcard ./*.cpp ./myListeners/*.cpp ./myResponsers/*.cpp)
CC_SOURCES = $(wildcard ./logout/*.c)

XX_OBJECTS = $(patsubst %.cpp,%.o,$(XX_SOURCES))
CC_OBJECTS = $(patsubst %.c,%.o,$(CC_SOURCES))

TARGET = appMain

$(TARGET) : $(XX_OBJECTS) $(CC_OBJECTS)
	$(XX) $^ -o $@ $(CLIBS)
	
$(XX_OBJECTS) : %.o : %.cpp
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

$(CC_OBJECTS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

ECHO:
	@echo $(SUBDIRS)
	@echo begin compile

.PHONY : clean
clean:
	rm -rf $(TARGET) $(XX_OBJECTS) $(CC_OBJECTS)
