LIB_DIR = ./../lib/
TOP_DIR = ./..

CC = gcc

TARGET = libLogout.a

SOURCES = $(wildcard ./*.c)
#SOURCES = $(wildcard ./*.cpp)
INCLUDE_DIRS = -I./ #指定头文件目录，代码中就不需要把头文件的完整路径写出来了

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
#OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET) : $(OBJECTS)
	$(AR) $(ARFLAG) $@ $^
	cp $@ $(LIB_DIR)
	
#$(OBJECTS) : %.o : %.cpp 
$(OBJECTS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS) 