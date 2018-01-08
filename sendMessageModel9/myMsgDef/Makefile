INCLUDE_DIRS = -I./ #指定头文件目录，代码中就不需要把头文件的完整路径写出来了

MSG_DEF_O = msgDef.pb.o
MSG_DEF_CC = msgDef.pb.cc
PROTO_XX = protoc

TARGET = test_proto_buffer

$(TARGET) :  $(MSG_DEF_O)
	$(XX) $(TARGET).cpp $(MSG_DEF_CC) -o $(TARGET) -lprotobuf

#protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto
	
$(MSG_DEF_O) : $(MSG_DEF_CC)
	$(XX) -c $(CFLAGS) $< -o $@ $(INCLUDE_DIRS)
$(MSG_DEF_CC) :
	$(PROTO_XX) -I=./ --cpp_out=./ msgDef.proto


.PHONY : clean
clean:
	rm -rf $(TARGET) $(MSG_DEF_O) 
