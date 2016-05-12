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

		Request request;
		Response response;

		{ //add user
			request.querytype = QueryType::ADD_USER;
			User user;
			user.name = "user_1";
			request.__set_add_user_user(user);

			client.process(response, request);

			std::cout << "id_client = " << response.add_user_user_id
					<< std::endl;
		}
		{ // get user
			request.querytype = QueryType::GET_USER;
			User user1;
			long id = 1;
			request.__set_get_user_user_id(id);
			client.process(response, request);
			printf("user1: %ld  %s\n", user1.id,
					response.get_user_user.name.c_str());
		}

		{ // add 9 users
			for (int i = 2; i <= 10; i++) {
				request.querytype = QueryType::ADD_USER;
				User tu;
				std::stringstream ss;
				ss << i;
				std::string ts;
				ss >> ts;
				tu.name = "user_" + ts;
				request.__set_add_user_user(tu);
				client.process(response, request);
			}
		}
		{ // get all users

			request.querytype = QueryType::GET_ALL_USER;

			client.process(response, request);
			std::vector<User> users = response.get_all_user_user_list;

			for (unsigned int i = 0; i < users.size(); i++) {
				printf("%ld  %s\n", users[i].id, users[i].name.c_str());
			}
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
		Request request;
		Response response;
		{ // add word
			request.querytype = QueryType::ADD_WORD;
			Word word;
			word.name = "word_1";
			request.__set_add_word_word(word);
			client.process(response, request);
			std::cout << "id_client_word = " << response.add_word_word_id
					<< std::endl;
		}
		{ // get word
			request.querytype = QueryType::GET_WORD;
			Word word1;
			long id = 1;
			request.__set_get_word_word_id(id);
			client.process(response, request);
			printf("%ld  %s\n", word1.id, response.get_word_word.name.c_str());

		}

		{ //add 9 words
			for (int i = 2; i <= 10; i++) {
				request.querytype = QueryType::ADD_WORD;
				Word tw;
				std::stringstream ss;
				ss << i;
				std::string ts;
				ss >> ts;
				tw.name = "word_" + ts;

				request.__set_add_word_word(tw);
				client.process(response, request);
			}
		}

		{ // get all words
			request.querytype = QueryType::GET_ALL_WORD;
			client.process(response, request);
			std::vector<Word> words = response.get_all_word_word_list;
			for (unsigned int i = 0; i < words.size(); i++) {
				printf("%ld  %s\n", words[i].id, words[i].name.c_str());
			}
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
		Request request;
		Response response;

		{ // add token
			Token token;
			token.name = "AB";
			TokenItem ti;
			ti.userId = 1;
			ti.wordId = 1;
			ti.postion = 100;
			ti.length = 999;
			token.tokenItemVetor.push_back(ti);

			request.querytype = QueryType::ADD_TOKEN;

			request.__set_add_token_token(token);
			client.process(response, request);

		}
		{ // get token
			request.querytype = QueryType::GET_TOKEN;
			request.__set_get_token_token_name("AB");

			client.process(response, request);

			std::cout << response.get_token_token.name << " "
					<< response.get_token_token.tokenItemVetor[0].length
					<< std::endl;
		}
		{ // get all tokens

			request.querytype = QueryType::GET_ALL_TOKEN;
			client.process(response, request);
			std::vector<Token> tokens = response.get_all_token_token_list;
			std::cout << "tokens.size() = " << tokens.size() << std::endl;
			for (unsigned int i = 0; i < tokens.size(); i++) {
				printf("all %s %d\n", tokens[i].name.c_str(),
						tokens[i].tokenItemVetor[0].length);
			}
		}

		transport->close();
	} catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}

