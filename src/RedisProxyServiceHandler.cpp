#include "RedisProxyServiceHandler.h"
RedisProxyServiceHandler::RedisProxyServiceHandler() {
	this->redisProxy = shared_ptr<RedisProxy>(
			new RedisProxy("localhost", 6379));
}

void RedisProxyServiceHandler::process(Response& response,
		const Request& request) {

	switch (request.querytype) {
	case QueryType::GET_USER:
		std::cout << "GET USER" << std::endl;
		{
			long id = request.get_user_user_id;
			boost::shared_ptr<User> user = this->redisProxy->getUserById(id);
			response.__set_get_user_user(*user);
		}
		break;
	case QueryType::ADD_USER:
		std::cout << "ADD_USER" << std::endl;
		{
			User user = request.add_user_user;
			User user_t;
			user_t.id = user.id;
			user_t.name = user.name;
			long id = this->redisProxy->addUser(user_t);

			response.__set_add_user_user_id(id);
		}
		break;
	case QueryType::GET_ALL_USER:
		std::cout << "GET_ALL_USER" << std::endl;
		{
			std::vector<User> users;
			this->redisProxy->getAllUser(users);

			response.__set_get_all_user_user_list(users);
		}
		break;
	case QueryType::ADD_WORD:
		std::cout << "ADD_WORD" << std::endl;
		{
			Word word = request.add_word_word;
			Word word_t;
			word_t.id = word.id;
			word_t.name = word.name;
			long id = this->redisProxy->addWord(word_t);
			response.__set_add_word_word_id(id);
		}
		break;
	case QueryType::GET_WORD:
		std::cout << "GET_WORD" << std::endl;
		{
			long id = request.get_word_word_id;
			boost::shared_ptr<Word> word = this->redisProxy->getWordById(id);
			response.__set_get_word_word(*word);
		}
		break;
	case QueryType::GET_ALL_WORD:
		std::cout << "GET_ALL_WORD" << std::endl;
		{
			std::vector<Word> words;
			this->redisProxy->getAllWord(words);
			response.__set_get_all_word_word_list(words);
		}
		break;
	case QueryType::ADD_TOKEN:
		std::cout << "ADD_TOKEN" << std::endl;
		{
			Token token = request.add_token_token;
			Token token_t;
			token_t.name = token.name;
			token_t.tokenItemVetor = token.tokenItemVetor;
			this->redisProxy->addToken(token_t);
		}
		break;

	case QueryType::GET_TOKEN:
		std::cout << "GET_TOKEN" << std::endl;
		{
			std::string token_name = request.get_token_token_name;
			boost::shared_ptr<Token> token = this->redisProxy->getTokenByName(
					token_name);
			if (token == NULL) {
				token = boost::shared_ptr<Token>(new Token);
			}
			response.__set_get_token_token(*token);
		}
		break;
	case QueryType::GET_ALL_TOKEN:
		std::cout << "GET_ALL_TOKEN" << std::endl;
		{
			std::vector<Token> tokens;
			this->redisProxy->getAllToken(tokens);

//			std::cout << "tokens.size()" << tokens.size() << std::endl;
			response.__set_get_all_token_token_list(tokens);
		}
		break;

	default:
		break;
	}
}
