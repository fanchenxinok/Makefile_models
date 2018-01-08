XX = g++
CFLAGS = -g 
CLIBS = -lpthread

INCLUDE = $(wildcard ./*.h) # INCLUDE = a.h b.h ... can't be defined like "INCLUDE = ./*.h"
SOURCES = $(wildcard ./*.cpp)

TARGET = mainApp
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET) : $(OBJECTS)
	$(XX) $(CFLAGS) $^ -o $@ $(CLIBS)
$(OBJECTS) : %.o : %.cpp
	$(XX) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm -rf *.o mainApp  
