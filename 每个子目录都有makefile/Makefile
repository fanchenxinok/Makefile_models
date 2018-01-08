XX = g++
AR = ar
ARFLAG = -rcs
CFLAGS = -g 

CLIBS = -L./lib/ -lSender -lReceiver -lResponser  -lpthread

SUBDIRS = ./receiver ./responser ./sender

INCLUDES = $(wildcard ./include/*.h ./sender/*.h ./receiver/*.h ./responser/*.h) # INCLUDE = a.h b.h ... can't be defined like "INCLUDE = ./*.h"
#SOURCES = $(wildcard ./*.cpp ./sender/*.cpp ./receiver/*.cpp ./responser/*.cpp)
INCLUDE_DIRS = -I./include -I./sender/ -I./receiver/ -I./responser/ #指定头文件目录，代码中就不需要把头文件的完整路径写出来了

TARGET = mainApp
#OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
OBJECTS = main.o

export XX CFLAGS AR ARFLAG

$(TARGET) : $(OBJECTS) $(SUBDIRS)
	$(XX) $< -o $@ $(CLIBS)     # $< 表示依赖列表的第一个 也就是 $(OBJECTS)
	
$(OBJECTS) : %.o : %.cpp 
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)
	
$(SUBDIRS):ECHO
	+$(MAKE) -C $@

ECHO:
	@echo $(SUBDIRS)
	@echo begin compile

.PHONY : clean
clean:
	for dir in $(SUBDIRS);\
	do $(MAKE) -C $$dir clean||exit 1;\
	done
	rm -rf $(TARGET) $(OBJECTS)  ./lib/*.a
