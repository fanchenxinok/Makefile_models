LIB_DIR = ./../lib/
TOP_DIR = ./..

SOURCES = $(wildcard ./*.cpp)
INCLUDE_DIRS = -I$(TOP_DIR)/include -I./ #指定头文件目录，代码中就不需要把头文件的完整路径写出来了

MSG_DEF_CC = msgDef.pb.cc
MSG_DEF_H = msgDef.pb.h

TARGET_LIB = libMsgDef.a
TARGET_APP = test_proto_buffer

PROTO_XX = protoc
#protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto

#OBJECTS = $(patsubst %.cc,%.o,$(SOURCES))
OBJECTS = msgDef.pb.o
	
$(TARGET_LIB) : $(OBJECTS)
	$(AR) $(ARFLAG) $@ $^
	cp $@ $(LIB_DIR)
	
$(OBJECTS) : %.o : $(MSG_DEF_CC)
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)
	$(XX) $(TARGET_APP).cpp $(MSG_DEF_CC) -o $(TARGET_APP) -lprotobuf

$(MSG_DEF_CC) :
	$(PROTO_XX) -I=./ --cpp_out=./ msgDef.proto


.PHONY : clean
clean:
	rm -rf $(TARGET_APP) $(TARGET_LIB) $(OBJECTS) 
