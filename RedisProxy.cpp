/*
 * RedisProxy.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: liang
 */

#include "RedisProxy.h"

RedisProxy::RedisProxy(std::string host, int port) {
	this->redis = boost::shared_ptr<Redis>(new Redis());
	if (!this->redis->connect(host, port)) {
		printf("connect error!\n");
		exit(0);
	}
}

RedisProxy::~RedisProxy() {
}

long RedisProxy::addUser(User user) {
	std::string USER_NUM_key;
	std::stringstream ss;
	ss << ConstValue::USER_NUM;
	ss >> USER_NUM_key;

	std::string user_id_str;
	bool redis_tag = this->redis->get(USER_NUM_key, user_id_str);

	if (redis_tag == false) {
		user.id = 1;
//		this->redis->set(USER_NUM_key, user.id);
	} else {
		long userNum;
		std::stringstream ss1;
		ss1 << user_id_str;
		ss1 >> userNum;
		user.id = userNum + 1;

	}

	std::stringstream idSs, valueSs;
	idSs << user.id;
	std::string USER_PERFIX_key = ConstValue::USER_PERFIX + "_" + idSs.str();

	boost::property_tree::ptree item;
	item.put("id", idSs.str());
	item.put("name", user.name);
	boost::property_tree::write_json(valueSs, item);
	//	std::cout << "aaaaa" << valueSs.str() << std::endl;
	std::string value = valueSs.str();

	this->redis->set(USER_PERFIX_key, value);

	{ //update user num
		std::stringstream user_num_ss;
		user_num_ss << user.id;
		this->redis->set(USER_NUM_key, user_num_ss.str());
	}

	return user.id;
}

boost::shared_ptr<User> RedisProxy::getUserById(long id) {
	std::stringstream idSs;
	idSs << id;
	std::string key = ConstValue::USER_PERFIX + "_" + idSs.str();
	std::string value;
	bool redis_tag = this->redis->get(key, value);

	boost::shared_ptr<User> user;
	if (redis_tag == true) {
		boost::property_tree::ptree items;

		std::stringstream valueSs(value);
		boost::property_tree::read_json<boost::property_tree::ptree>(valueSs,
				items);
		user = boost::shared_ptr<User>(new User());
		user->id = id;
		user->name = items.get_child("name").get_value("name");
		return user;
	} else {
		return user;
	}
}

boost::shared_ptr<User> RedisProxy::getUserByKey(std::string key) {
	std::string value;
	bool redis_tag = this->redis->get(key, value);

	boost::shared_ptr<User> user;
	if (redis_tag == true) {
		boost::property_tree::ptree items;

		std::stringstream valueSs(value);
		boost::property_tree::read_json<boost::property_tree::ptree>(valueSs,
				items);

		std::string sid = items.get_child("id").get_value("id");
		std::stringstream ss(sid);
		long id;
		ss >> id;
		user = boost::shared_ptr<User>(new User());
		user->id = id;
		user->name = items.get_child("name").get_value("name");
		return user;
	} else {
		return user;
	}
}

bool RedisProxy::getAllUser(std::vector<User> &users) {
	std::list<std::string> keyList;

	bool tag = this->redis->getPrefixKey(ConstValue::USER_PERFIX, keyList);
	if (tag == false) {
		return tag;
	}

	for (std::list<std::string>::iterator it = keyList.begin();
			it != keyList.end(); it++) {
		std::string s = *(it);
		boost::shared_ptr<User> user = this->getUserByKey(s);
		if (user == NULL) {
			return false;
		} else {
			users.push_back(*user);
		}
	}
	return tag;
}

boost::shared_ptr<Word> RedisProxy::getWordByKey(std::string key) {
	std::string value;
	bool redis_tag = this->redis->get(key, value);

	boost::shared_ptr<Word> word;
	if (redis_tag == true) {
		boost::property_tree::ptree items;

		std::stringstream valueSs(value);
		boost::property_tree::read_json<boost::property_tree::ptree>(valueSs,
				items);

		std::string sid = items.get_child("id").get_value("id");
		std::stringstream ss(sid);
		long id;
		ss >> id;
		word = boost::shared_ptr<Word>(new Word());
		word->id = id;
		word->name = items.get_child("name").get_value("name");
		return word;
	} else {
		return word;
	}
}

bool RedisProxy::getAllWord(std::vector<Word> &words) {
	std::list<std::string> keyList;
	bool tag = this->redis->getPrefixKey(ConstValue::WORD_PERFIX, keyList);
	if (tag == false) {
		return tag;
	}

	for (std::list<std::string>::iterator it = keyList.begin();
			it != keyList.end(); it++) {
		std::string s = *(it);
		boost::shared_ptr<Word> word = this->getWordByKey(s);
		if (word == NULL) {
			return false;
		} else {
			words.push_back(*word);
		}
	}

	return tag;

}
long RedisProxy::addWord(Word word) {
	std::string WORD_NUM_key;
	std::stringstream ss;
	ss << ConstValue::WORD_NUM;
	ss >> WORD_NUM_key;
	std::string s_wordId;
	bool redis_tag = this->redis->get(WORD_NUM_key, s_wordId);
	if (redis_tag == false) {
		word.id = 1;
	} else {
		long wordNum;
		std::stringstream ss1;
		ss1 << s_wordId;
		ss1 >> wordNum;
		word.id = wordNum + 1;
	}

	std::stringstream idSs, valueSs;
	idSs << word.id;
	std::string WORD_PERFIX_key = ConstValue::WORD_PERFIX + "_" + idSs.str();

	boost::property_tree::ptree item;
	item.put("id", idSs.str());
	item.put("name", word.name);
	boost::property_tree::write_json(valueSs, item);
	//	std::cout << "aaaaa" << valueSs.str() << std::endl;
	std::string value = valueSs.str();

	this->redis->set(WORD_PERFIX_key, value);

	{ //update word num
		std::stringstream word_num_ss;
		word_num_ss << word.id;
		this->redis->set(WORD_NUM_key, word_num_ss.str());
	}
	return word.id;
}

boost::shared_ptr<Word> RedisProxy::getWordById(long id) {
	std::stringstream idSs;
	idSs << id;
	std::string key = ConstValue::WORD_PERFIX + "_" + idSs.str();
	std::string value;
	bool redis_tag = this->redis->get(key, value);

	boost::shared_ptr<Word> word;
	if (redis_tag == true) {
		boost::property_tree::ptree items;

		std::stringstream valueSs(value);
		boost::property_tree::read_json<boost::property_tree::ptree>(valueSs,
				items);

		word = boost::shared_ptr<Word>(new Word());
		word->id = id;
		word->name = items.get_child("name").get_value("name");
		return word;
	} else {
		return word;
	}
}

void RedisProxy::addToken(Token token) {
	std::stringstream idSs, valueSs;
	idSs << token.name;
	std::string key = ConstValue::TOKEN_PERFIX + "_" + idSs.str();

	boost::property_tree::ptree items;
	boost::property_tree::ptree tokenItemListItems;
	//	boost::pro

	std::vector<TokenItem> tokenItemVector = token.tokenItemVetor;
	for (unsigned int i = 0; i < tokenItemVector.size(); i++) {
		boost::property_tree::ptree tokenItem;
		tokenItem.put("userId", tokenItemVector[i].userId);
		tokenItem.put("wordId", tokenItemVector[i].wordId);
		tokenItem.put("postion", tokenItemVector[i].postion);
		tokenItem.put("length", tokenItemVector[i].length);
		tokenItemListItems.push_back(std::make_pair("", tokenItem));
	}

	items.put("name", token.name);
	items.put_child("tokenItemVector", tokenItemListItems);
	boost::property_tree::write_json(valueSs, items);

	std::string value = valueSs.str();

	this->redis->set(key, value);
}

boost::shared_ptr<Token> RedisProxy::getTokenByKey(std::string key) {
	std::string value_t;
	bool redis_tag = this->redis->get(key, value_t);
	boost::shared_ptr<Token> token;

	if (redis_tag == false) {
		return token;
	} else {
		token = boost::shared_ptr<Token>(new Token());

		int index = key.rfind('_');
		token->name = key.substr(index + 1);
		std::string value = value_t;

		std::stringstream ss;
		ss << value;

		boost::property_tree::ptree items;
		boost::property_tree::read_json<boost::property_tree::ptree>(ss, items);

		boost::property_tree::ptree tokenItemListPtree = items.get_child(
				"tokenItemVector");

		for (boost::property_tree::ptree::iterator it =
				tokenItemListPtree.begin(); it != tokenItemListPtree.end();
				it++) {
			long userId = it->second.get<long>("userId");
			long wordId = it->second.get<long>("wordId");
			int postion = it->second.get<int>("postion");
			int length = it->second.get<int>("length");
			TokenItem ti;
			ti.userId = userId;
			ti.wordId = wordId;
			ti.postion = postion;
			ti.length = length;
			token->tokenItemVetor.push_back(ti);
		}
		return token;
	}
}

boost::shared_ptr<Token> RedisProxy::getTokenByName(std::string name) {

	std::string key = ConstValue::TOKEN_PERFIX + "_" + name;
	std::string value_t;
	bool redis_tag = this->redis->get(key, value_t);
	boost::shared_ptr<Token> token;

	if (redis_tag == false) {
		return token;
	} else {
		token = boost::shared_ptr<Token>(new Token());
		token->name = name;
		std::string value = value_t;

		std::stringstream ss;
		ss << value;

		boost::property_tree::ptree items;
		boost::property_tree::read_json<boost::property_tree::ptree>(ss, items);

		boost::property_tree::ptree tokenItemListPtree = items.get_child(
				"tokenItemVector");

		for (boost::property_tree::ptree::iterator it =
				tokenItemListPtree.begin(); it != tokenItemListPtree.end();
				it++) {
			long userId = it->second.get<long>("userId");
			long wordId = it->second.get<long>("wordId");
			int postion = it->second.get<int>("postion");
			int length = it->second.get<int>("length");
			TokenItem ti;
			ti.userId = userId;
			ti.wordId = wordId;
			ti.postion = postion;
			ti.length = length;
			token->tokenItemVetor.push_back(ti);
		}
		return token;
	}

}
bool RedisProxy::getAllToken(std::vector<Token> &tokens) {
	std::list<std::string> keyList;
	bool tag = this->redis->getPrefixKey(ConstValue::TOKEN_PERFIX, keyList);
	if (tag == false) {
		return tag;
	}

	for (std::list<std::string>::iterator it = keyList.begin();
			it != keyList.end(); it++) {
		boost::shared_ptr<Token> token = this->getTokenByKey(*(it));
		if (token == NULL) {
			return false;
		} else {
			tokens.push_back(*token);
		}
	}

	return tag;
}
//void RedisProxy::userBuyWord(long userId, long wordId) {
//	boost::shared_ptr<Word> word = this->getWordById(wordId);
//	std::string s = word->name;
//
//	for (unsigned int i = 0; i < s.size();) {
//		int postion = i;
//		std::string tokenName;
//		if (s[i] >= ConstValue::CHINA_START && s[i] <= ConstValue::CHINA_END) {
//			if (i + 3 < s.size()) {
//				tokenName.append(s.substr(i, 3));
//				i += 3;
//			} else {
//				break;
//			}
//		} else {
//			if (i + 1 < s.size()) {
//				tokenName.append(s.substr(i, 1));
//				i += 1;
//			} else {
//				break;
//			}
//		}
//
//		if (s[i] >= ConstValue::CHINA_START && s[i] <= ConstValue::CHINA_END) {
//			tokenName.append(s.substr(i, 3));
//		} else {
//			tokenName.append(s.substr(i, 1));
//		}
//
//		boost::shared_ptr<Token> token = this->getTokenByName(tokenName);
//		if (token == NULL) {
//			token = boost::shared_ptr<Token>(new Token());
//			token->name = tokenName;
//		}
//
//		TokenItem tokenItem;
//		tokenItem.userId = userId;
//		tokenItem.wordId = wordId;
//		tokenItem.postion = postion;
//		tokenItem.length = s.size();
//
//		token->tokenItemVetor.push_back(tokenItem);
//		this->addToken(*token);
//	}
//}
//
//std::vector<TokenItem> RedisProxy::generateTokenItemVectorThroughSearchString(
//		const std::string& ws) {
//
//	std::vector<TokenItem> tokenItemVector;
//	std::vector<std::string> vs = this->cutString(ws);
//
//	for (unsigned int i = 0; i < vs.size() && i + 1 < vs.size(); i += 2) {
//		std::string tokenName = vs[i] + vs[i + 1];
//
////		std::cout << "tokenName: " << tokenName << std::endl;
//
//		boost::shared_ptr<Token> token = this->getTokenByName(tokenName);
//		if (token != NULL) {
//			for (unsigned int i = 0; i < token->tokenItemVetor.size(); i++) {
//				tokenItemVector.push_back(token->tokenItemVetor[i]);
//			}
//		}
//	}
//
//	if (vs.size() % 2 == 1 && vs.size() > 2) {
//		unsigned int i = vs.size() - 2;
//		std::string tokenName = vs[i] + vs[i + 1];
////		std::cout << "tokenName: " << tokenName << std::endl;
//		boost::shared_ptr<Token> token = this->getTokenByName(tokenName);
//		if (token != NULL) {
//			for (unsigned int i = 0; i < token->tokenItemVetor.size(); i++) {
//				tokenItemVector.push_back(token->tokenItemVetor[i]);
//			}
//		}
//	}
//
//	return tokenItemVector;
//}
//
//std::map<std::pair<long, long>, std::priority_queue<std::pair<int, int> > > RedisProxy::generateMapThroughTokenItemVector(
//		const std::vector<TokenItem>& tokenItemVector) {
//	std::map<std::pair<long, long>, std::priority_queue<std::pair<int, int> > > map;
//	for (unsigned int i = 0; i < tokenItemVector.size(); i++) {
//		std::pair<long, long> p_key(tokenItemVector[i].userId,
//				tokenItemVector[i].wordId);
//		std::priority_queue<std::pair<int, int> > p_queue = map[p_key];
//		p_queue.push(
//				std::pair<int, int>(tokenItemVector[i].postion,
//						tokenItemVector[i].length));
//		map[p_key] = p_queue;
//	}
//	return map;
//}
//
//std::vector<User> RedisProxy::getChargeUsersThroughMap(
//		std::map<std::pair<long, long>,
//				std::priority_queue<std::pair<int, int> > > map,
//		const std::string& ws) {
//	std::vector<User> userVector;
//	for (std::map<std::pair<long, long>,
//			std::priority_queue<std::pair<int, int> > >::iterator it =
//			map.begin(); it != map.end(); it++) {
//		std::priority_queue<std::pair<int, int> > p_queue = it->second;
//		if (p_queue.empty() == false) {
//			//			unsigned int length = p_queue.top().second;
//			bool beAdd = true;
//			unsigned int start = p_queue.top().first;
//			unsigned int end = start;
//			if (ws[end] >= ConstValue::CHINA_START
//					&& ws[end] <= ConstValue::CHINA_END) {
//				end += 3;
//			} else {
//				end += 1;
//			}
//			if (ws[end] >= ConstValue::CHINA_START
//					&& ws[end] <= ConstValue::CHINA_END) {
//				end += 3;
//			} else {
//				end += 1;
//			}
//
//			bool queue_first = true;
//			while (p_queue.empty() == false) {
//				std::pair<int, int> p = p_queue.top();
//				p_queue.pop();
//				start = p.first;
//				if (ws[start] >= ConstValue::CHINA_START
//						&& ws[start] <= ConstValue::CHINA_END) {
//					start += 3;
//				} else {
//					start += 1;
//				}
//				int end_t = start;
//				if (ws[start] >= ConstValue::CHINA_START
//						&& ws[start] <= ConstValue::CHINA_END) {
//					start += 3;
//				} else {
//					start += 1;
//				}
//				if (start != end) {
//					beAdd = false;
//					break;
//				} else {
//					std::vector<std::string> vs = this->cutString(ws);
//					if (queue_first == true && vs.size() % 2 == 1) {
//						end = end_t;
//						queue_first = false;
//					} else {
//						end = p.first;
//					}
//				}
//			}
//			if (beAdd == true) {
//				userVector.push_back(*(this->getUserById(it->first.first)));
//			}
//		}
//	}
//	return userVector;
//}
//
//std::vector<User> RedisProxy::produceSearchResult(std::string ws) {
//	std::vector<TokenItem> tokenItemVector =
//			generateTokenItemVectorThroughSearchString(ws);
//
//	for (unsigned int i = 0; i < tokenItemVector.size(); i++) {
//		printf("{%d, %d, %d, %d}", tokenItemVector[i].userId,
//				tokenItemVector[i].wordId, tokenItemVector[i].length,
//				tokenItemVector[i].length);
//	}
//	std::map<std::pair<long, long>, std::priority_queue<std::pair<int, int> > > map =
//			generateMapThroughTokenItemVector(tokenItemVector);
//
//	std::vector<User> userVector = getChargeUsersThroughMap(map, ws);
//	return userVector;
//}
//
//std::vector<std::string> RedisProxy::cutString(std::string ws) {
//	std::vector<std::string> ;
//	for (unsigned int i = 0; i < ws.size();) {
//		if (ws[i] >= ConstValue::CHINA_START
//				&& ws[i] <= ConstValue::CHINA_END) {
//			v.push_back(ws.substr(i, 3));
//			i += 3;
//		} else {
//			v.push_back(ws.substr(i, 1));
//			i += 1;
//		}
//	}
//	return v;
//}

void RedisProxy::cleanAllKey() {
	this->redis->cleanAllKey();
}

