XX = g++
CFLAGS = -g 
CLIBS = -lpthread

INCLUDES = $(wildcard ./include/*.h ./sender/*.h ./receiver/*.h ./responser/*.h) # INCLUDE = a.h b.h ... can't be defined like "INCLUDE = ./*.h"
SOURCES = $(wildcard ./*.cpp ./sender/*.cpp ./receiver/*.cpp ./responser/*.cpp)
INCLUDE_DIRS = -I./include -I./sender/ -I./receiver/ -I./responser/ #指定头文件目录，代码中就不需要把头文件的完整路径写出来了

TARGET = mainApp
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET) : $(OBJECTS)
	$(XX) $(CFLAGS) $^ -o $@ $(CLIBS)
$(OBJECTS) : %.o : %.cpp 
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS)  
