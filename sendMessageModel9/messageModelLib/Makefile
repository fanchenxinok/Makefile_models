XX = g++
AR = ar
ARFLAG = -rcs
# 静态库链接成.so动态库，编译静态库的时候需要加 -fPIC这个参数。
CFLAGS = -g -fPIC

STATIC_LIBS = -lSender -lReceiver -lResponser -lListener -lLogout
#CLIBS = $(addprefix -L./lib/,$(STATIC_LIBS))
#CLIBS += -lpthread -lrt
CLIBS = -L./lib -lSender -lReceiver -lResponser -lListener -lLogout -lpthread -lrt #-lMsgDef

SUBDIRS = ./logout ./receiver ./responser ./sender ./listener #./msgDef
 # INCLUDE = a.h b.h ... can't be defined like "INCLUDE = ./*.h"
INCLUDES = $(wildcard ./include/*.h ./sender/*.h ./receiver/*.h ./responser/*.h)
#指定头文件目录，代码中就不需要把头文件的完整路径写出来了
INCLUDE_DIRS = -I./include -I./sender/ -I./receiver/ -I./responser/ -I./listener -I./logout 

OBJECTS = mainApi.o

export XX CFLAGS AR ARFLAG

TARGET = libSendMsgModel.so          


$(TARGET) : $(OBJECTS) $(SUBDIRS)
	$(XX) -shared -fPIC $< -o $@ $(CLIBS)     # $< 表示依赖列表的第一个 也就是 $(OBJECTS)
	cp $@ ../lib
	sudo cp $@ /usr/lib
	
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
	rm -rf libSendMsgModel* $(OBJECTS)  ./lib/*.a
