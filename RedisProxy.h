/*
 * RedisProxy.h
 *
 *  Created on: Apr 13, 2016
 *      Author: liang
 */

#include "query_engin_G2_types.h"
#include "Redis.h"
#include "ConstValue.h"
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <stdio.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>

#ifndef REDISPROXY_H_
#define REDISPROXY_H_

class RedisProxy {
public:
	RedisProxy(std::string host, int port);
	virtual ~RedisProxy();

	long addUser(User user);
	boost::shared_ptr<User> getUserById(long id);
	boost::shared_ptr<User> getUserByKey(std::string key);
	bool getAllUser(std::vector<User> &users);
	bool getAllToken(std::vector<Token> &tokens);
	long addWord(Word word);
	boost::shared_ptr<Word> getWordById(long id);
	boost::shared_ptr<Word> getWordByKey(std::string key);
	bool getAllWord(std::vector<Word> &words);
	void addToken(Token token);
	boost::shared_ptr<Token> getTokenByName(std::string s);
	boost::shared_ptr<Token> getTokenByKey(std::string key);
	void cleanAllKey();

	void userBuyWord(long userId, long wordId);

	//get the user list who will be charged for that someone search the word in parameter "ws"
	std::vector<User> produceSearchResult(std::string ws);
private:
	boost::shared_ptr<Redis> redis;
//	std::vector<std::string> cutString(std::string ws);
//	std::vector<TokenItem> generateTokenItemVectorThroughSearchString(
//			const std::string& ws);
//	std::map<std::pair<long, long>, std::priority_queue<std::pair<int, int> > > generateMapThroughTokenItemVector(
//			const std::vector<TokenItem>& tokenItemVector);
//	std::vector<User> getChargeUsersThroughMap(
//			std::map<std::pair<long, long>,
//					std::priority_queue<std::pair<int, int> > > map,
//			const std::string& ws);
};

#endif /* REDISPROXY_H_ */
