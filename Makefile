CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

vpath %.cpp src:test

INCLUDE = -Iinclude

THIFT_LIBS = -lthrift
REDIS_LIBS = -lhiredis
CONFIG_LIBS = -lconfig++
TEST_LIBS = -pthread -lgtest -lhiredis
BOST_LIBS = -lboost_system -lboost_thread -lboost_filesystem



THIFT_OBJS = query_engin_G2_constants.o query_engin_G2_types.o RedisProxyService.o  QueryEngineServer.o

REDIS_OBJ  = Redis.o RedisProxy.o ConstValue.o

SERVER_OBJS = Configure.o RedisProxyServiceHandler.o main.o 




SERVER_TARGET = server_main  



TEST_TARGET = testMain
TEST_OBJS = test.o testMain.o

all: $(SERVER_TARGET)


$(TEST_TARGET): $(THIFT_OBJS) $(TEST_OBJS)
	$(CXX) -o $(TEST_TARGET) $(THIFT_OBJS) $(TEST_OBJS) $(TEST_LIBS) $(THIFT_LIBS) \
	 $(CONFIG_LIBS) $(INCLUDE)


$(SERVER_TARGET):	$(THIFT_OBJS) $(SERVER_OBJS) $(REDIS_OBJ)
	$(CXX) -o $(SERVER_TARGET) $(THIFT_OBJS) $(SERVER_OBJS)  $(REDIS_OBJ) $(THIFT_LIBS) \
	$(REDIS_LIBS) $(CONFIG_LIBS) $(BOST_LIBS) $(INCLUDE)
	mv $(SERVER_TARGET) ./bin
	

%.o : %.cpp
	${CXX} ${CFLAGS} -c $< -o $@ $(INCLUDE)

	
server: $(SERVER_TARGET)
	@echo "server start..."
	@./bin/$(SERVER_TARGET)
clean:
	rm -f *.o ./bin/$(SERVER_TARGET)  $(TEST_TARGET)
	
	
test: $(TEST_TARGET)
	@./$(TEST_TARGET)
