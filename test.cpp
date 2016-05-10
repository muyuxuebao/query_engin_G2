#include <gtest/gtest.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "RedisProxyService.h"
#include "Redis.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class Gtest: public ::testing::Test {

protected:
	Redis *redis;
	virtual void SetUp() {
		std::cout << "Test  begin  \n";

		this->redis = new Redis();
		if (!this->redis->connect("localhost", 6379)) {
			printf("connect error!\n");
			return;
		}

		this->redis->cleanAllKey();
	}
	virtual void TearDown() {
		std::cout << "Test end \n";
	}
};

TEST_F(Gtest, test_Redis) {
	bool tag;
	tag = this->redis->set("aaa", "bbb");
	std::cout << tag << std::endl;
	std::string value;
	tag = this->redis->get("aaa", value);
	std::cout << tag << std::endl;
	std::cout << value << std::endl;
}

TEST_F(Gtest, test_User) {
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RedisProxyServiceClient client(protocol);

	try {
		transport->open();
		User user;
		user.name = "user_1";
		client.addUser(user);

		User user1;
		client.getUser(user1, 1);
		printf("%ld  %s\n", user1.id, user1.name.c_str());

		for (int i = 2; i <= 10; i++) {
			User tu;
			std::stringstream ss;
			ss << i;
			std::string ts;
			ss >> ts;
			tu.name = "user_" + ts;
			client.addUser(tu);
		}

		std::vector<User> users;
		client.getAllUser(users);
		for (unsigned int i = 0; i < users.size(); i++) {
			printf("%ld  %s\n", users[i].id, users[i].name.c_str());
		}

		transport->close();
	} catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}

TEST_F(Gtest, test_Word) {
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RedisProxyServiceClient client(protocol);

	try {
		transport->open();
		Word word;
		word.name = "word_1";
		client.addWord(word);

		Word word1;
		client.getWord(word1, 1);
		printf("%ld  %s\n", word1.id, word1.name.c_str());

		for (int i = 2; i <= 10; i++) {
			Word tu;
			std::stringstream ss;
			ss << i;
			std::string ts;
			ss >> ts;
			tu.name = "word_" + ts;
			client.addWord(tu);
		}

		std::vector<Word> words;
		client.getAllWord(words);
		for (unsigned int i = 0; i < words.size(); i++) {
			printf("%ld  %s\n", words[i].id, words[i].name.c_str());
		}

		transport->close();
	} catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}

TEST_F(Gtest, test_Token) {
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	RedisProxyServiceClient client(protocol);

	try {
		transport->open();
		Token token;
		token.name = "AB";
		TokenItem ti;
		ti.userId = 1;
		ti.wordId = 1;
		ti.postion = 100;
		ti.length = 999;
		token.tokenItemVetor.push_back(ti);
		client.addToken(token);

		Token token1;
		client.getToken(token1, token.name);

		std::cout << token1.name << " " << token1.tokenItemVetor[0].length
				<< std::endl;

		transport->close();
	} catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}

//
//TEST_F(Gtest, test_UserBuyWordAndGetChargeUsers) {
//	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
//	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
//	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
//	RedisProxyServiceClient client(protocol);
//
//	try {
//		transport->open();
//
//		User user;
//		user.name = "wangermazi";
//		Word word;
//		word.name = "北A鲜B";
//		user.id = client.addUser(user);
//		word.id = client.addWord(word);
//		client.userBuyWord(user.id, word.id);
//
//		User user1;
//		user1.name = "zhangsan";
//		Word word1;
//		word1.name = "京鲜花盆";
//		user1.id = client.addUser(user1);
//		word1.id = client.addWord(word1);
//		client.userBuyWord(user1.id, word.id);
//
//		User user2;
//		user2.name = "lisi";
//		user2.id = client.addUser(user2);
//		client.userBuyWord(user.id, word1.id);
//
//		User user3;
//		user3.name = "wangwu";
//		user3.id = client.addUser(user3);
//		client.userBuyWord(user3.id, word1.id);
//
//		User user4;
//		user4.name = "xxx";
//		user4.id = client.addUser(user4);
//		client.userBuyWord(user4.id, word.id);
//
//		std::vector<User> userVector;
//		Word word_cu;
//		word_cu.name = "北A鲜B";
//		client.getChargeUsers(userVector, word_cu);
//
//		printf(
//				"****************************charge users*********************************\n");
//		for (unsigned int i = 0; i < userVector.size(); i++) {
//			printf("%ld, %s\n", userVector[i].id, userVector[i].name.c_str());
//		}
//
//		printf(
//				"****************************get All token*********************************\n");
//		std::vector<Token> tokens;
//		client.getAllToken(tokens);
//		for (unsigned int i = 0; i < tokens.size(); i++) {
//			std::cout << tokens[i].name << std::endl;
//		}
//
//		transport->close();
//	} catch (TException& tx) {
//		cout << "ERROR: " << tx.what() << endl;
//	}
//}

//TEST_F(Gtest, test_UserBuyWordAndGetChargeUsers2) {
//	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
//	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
//	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
//	RedisProxyServiceClient client(protocol);
//
//	try {
//		transport->open();
//
//		User user;
//		user.name = "wangermazi";
//		Word word;
//		word.name = "中华人民共和国";
//		user.id = client.addUser(user);
//		word.id = client.addWord(word);
//		client.userBuyWord(user.id, word.id);
//
//		User user1;
//		user1.name = "zhangsan";
//		Word word1;
//		word1.name = "中华人民";
//		user1.id = client.addUser(user1);
//		word1.id = client.addWord(word1);
//		client.userBuyWord(user1.id, word.id);
//
//		std::vector<User> userVector;
//		Word word_cu;
//		word_cu.name = "华人民共";
//		client.getChargeUsers(userVector, word_cu);
//
//		printf(
//				"****************************charge users*********************************\n");
//		for (unsigned int i = 0; i < userVector.size(); i++) {
//			printf("%ld, %s\n", userVector[i].id, userVector[i].name.c_str());
//		}
//
//		printf(
//				"****************************get All token*********************************\n");
//		std::vector<Token> tokens;
//		client.getAllToken(tokens);
//		for (unsigned int i = 0; i < tokens.size(); i++) {
//			std::cout << tokens[i].name << std::endl;
//		}
//
//		transport->close();
//	} catch (TException& tx) {
//		cout << "ERROR: " << tx.what() << endl;
//	}
//}
