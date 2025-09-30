CXX = g++
CXXFLAGS = -Wall -g
TARGET = dev_reloader
SRCS = main.cpp cpp/events_register.cpp cpp/gateway.cpp cpp/server.cpp cpp/watcher.cpp
OBJS = $(SRCS:.cpp=.o)
all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# run: $(TARGET)
# 	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
