CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

vpath %.cpp src:test

INCLUDE = -Iinclude

THIFT_LIBS = -lthrift
REDIS_LIBS = -lhiredis
LIBCONFIG_LIBS = -lconfig++
TEST_LIBS = -pthread -lgtest -lhiredis
BOST_LIBS = -lboost_system -lboost_thread -lboost_filesystem


LIBCONFIG_OBJS = Configure.o 

THIFT_OBJS = query_engin_G2_constants.o query_engin_G2_types.o RedisProxyService.o RedisProxyServiceHandler.o 

REDIS_OBJ  = Redis.o RedisProxy.o ConstValue.o

SERVER_OBJS = QueryEngineServer.o main.o




SERVER_TARGET = server_main  



TEST_TARGET = testMain
TEST_OBJS = test.o testMain.o

all: $(SERVER_TARGET)


$(TEST_TARGET): $(THIFT_OBJS) $(TEST_OBJS) $(REDIS_OBJ) $(LIBCONFIG_OBJS)
	$(CXX) -o ./bin/$(TEST_TARGET) $(THIFT_OBJS) $(REDIS_OBJ) $(TEST_OBJS) $(TEST_LIBS) $(THIFT_LIBS) \
	$(LIBCONFIG_LIBS) $(INCLUDE)


$(SERVER_TARGET): $(THIFT_OBJS) $(SERVER_OBJS) $(REDIS_OBJ) $(LIBCONFIG_OBJS)
	$(CXX) -o ./bin/$(SERVER_TARGET) $(THIFT_OBJS) $(SERVER_OBJS)  $(LIBCONFIG_OBJS) $(REDIS_OBJ) $(THIFT_LIBS) \
	$(REDIS_LIBS) $(LIBCONFIG_LIBS) $(BOST_LIBS) $(INCLUDE)
	

%.o : %.cpp
	${CXX} ${CFLAGS} -c $< -o $@ $(INCLUDE)

	
server: $(SERVER_TARGET)
	@echo "server start..."
	@./bin/$(SERVER_TARGET)
clean:
	rm -f *.o ./bin/$(SERVER_TARGET)  ./bin/$(TEST_TARGET)
	
	
test: $(TEST_TARGET)
	@./bin/$(TEST_TARGET)
