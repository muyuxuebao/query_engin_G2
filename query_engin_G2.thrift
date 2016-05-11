
struct User{
        1:i64 id, 
        2:string name
} 

struct Word{
        1:i64 id, 
        2:string name
} 
struct Token{
        1:string name,
        2:list<TokenItem> tokenItemVetor
} 
struct TokenItem{
        1:i64 userId, 
        2:i64 wordId,
        3:i32 postion,
        4:i32 length,
} 

enum QueryType{
    GET_USER,
    ADD_USER,
    GET_WORD,
    ADD_WORD,
	GET_TOKEN,
    ADD_TOKEN,
    
    GET_ALL_USER,
    GET_ALL_WORD,
    GET_ALL_TOKEN
}
struct  Request {
	1: required QueryType querytype
	2: optional i64 get_user_user_id, //get user
	3: optional User add_user_user,  //add user
	4: optional i64 get_word_word_id,//get word
	5: optional Word add_word_word, //add word
	6: optional Token add_token_token,//add token
	7: optional string get_token_token_name,//get token
}

struct  Response {
	1: required QueryType querytype
	
	2:optional User get_user_user,  //get user
	3:optional i64 add_user_user_id,//add user
	4:optional i64 add_word_word_id,//add word
	5:optional Word get_word_word,//get word
	
	6:optional Token get_token_token,//get token
	
	7:optional list<User> get_all_user_user_list, //get all user
	8:optional list<Word> get_all_word_word_list, //get all word
	9:optional list<Token> get_all_token_token_list  //get all token
	
}
 
service RedisProxyService{
	Response process(1:Request req);
}


