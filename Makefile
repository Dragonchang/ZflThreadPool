CC := g++
CFLAGS := -g

TARGET := out/MessageQueue
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %cpp,%o,$(SRCS))
OUT := out/

all:$(TARGET)
%.o:%.cpp
	mkdir -p $(OUT)
	$(CC) $(CFLAGS) -c $<
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

clean:
	rm -rf $(TARGET) *.o
