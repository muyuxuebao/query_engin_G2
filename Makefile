CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

THIFT_OBJS = query_engin_G2_constants.o query_engin_G2_types.o RedisProxyService.o

REDIS_OBJ  = Redis.o RedisProxy.o ConstValue.o

SERVER_OBJS = RedisProxyService_server.o 


THIFT_LIBS = -lthrift
REDIS_LIBS = -lhiredis

SERVER = server  



TEST_TARGET = testMain
TEST_LIBS = -pthread -lgtest -lhiredis
TEST_OBJS = test.o testMain.o

all: $(SERVER) $(TEST_TARGET)


$(TEST_TARGET): $(THIFT_OBJS) $(TEST_OBJS)
	$(CXX) -o $(TEST_TARGET) $(THIFT_OBJS) $(TEST_OBJS) $(TEST_LIBS) $(THIFT_LIBS)


$(SERVER):	$(THIFT_OBJS) $(SERVER_OBJS) $(REDIS_OBJ)
	$(CXX) -o $(SERVER) $(THIFT_OBJS) $(SERVER_OBJS)  $(REDIS_OBJ) $(THIFT_LIBS) $(REDIS_LIBS)
	

test: $(TEST_TARGET)
	@./$(TEST_TARGET)
clean:
	rm -f *.o $(SERVER)   $(TEST_TARGET)
