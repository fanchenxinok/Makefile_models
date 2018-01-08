LIB_DIR = ./../lib/
TOP_DIR = ./..

SOURCES = $(wildcard ./*.cpp)
INCLUDE_DIRS = -I$(TOP_DIR)/include -I./ #指定头文件目录，代码中就不需要把头文件的完整路径写出来了

TARGET = libListener.a
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET) : $(OBJECTS) #$(TOP_DIR)/logout/log_out.o
	$(AR) $(ARFLAG) $@ $^
	cp $@ $(LIB_DIR)
	
$(OBJECTS) : %.o : %.cpp 
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS) 