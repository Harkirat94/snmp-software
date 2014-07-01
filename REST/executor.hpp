#ifndef __EXECUTOR_FOR_API__
#define __EXECUTOR_FOR_API__

#include <string>
#include <set>
#include <vector>
#include "strutil.hpp"

using std::string;
using std::set;
using std::vector;

namespace ourapi
{

struct args_container;
struct duration_container;
void createNew(vector<struct duration_container>& durationRecords,pqxx::result::const_iterator cur_it);
std::ostream &operator<<(std::ostream &os, duration_container const &obj);

class Executor
{   
public:
    enum outputType {
        TYPE_JSON, TYPE_XML   
    };
    Executor();
	bool uid(const args_container & args, outputType type,  string& response);
  bool last(const args_container & args, outputType type,  string& response,const string & url);
  bool std(const args_container & args, outputType type,  string & response,const string & url);
private:
	bool generic_query(const vector<string>& args, outputType type,  string& response,string query,int t);
  bool generic_query(string & response, string query,unsigned int type);
};

#define AUID 1
#define AFROM 2
#define ATO 4
#define AFORMAT 8
#define ALAST 16
#define AMAC 32
#define VALID_API_STD 15  // Valid API "standard" - come up with better names
#define VALID_API_LAST 17 // Valid "last" n connections api
#define VALID_API_MAC 32 // Valid API to get the uid form the MAC


struct args_container{
  unsigned int type;
  string uid,from,to,format,last,mac;
  void erase_whitespace(){
    if(type == VALID_API_STD){
      StrUtil::eraseWhiteSpace(uid);
      StrUtil::eraseWhiteSpace(from);
      StrUtil::eraseWhiteSpace(to);
      StrUtil::eraseWhiteSpace(format);
    }
    else if(type == VALID_API_LAST){
      StrUtil::eraseWhiteSpace(uid);
      StrUtil::eraseWhiteSpace(last);
    }
    else if(type == VALID_API_MAC){
      StrUtil::eraseWhiteSpace(mac);
    }
  }
};

struct duration_container{
	int device_id;
	string label,from,to;	
	bool deauthFlag;
};

void createNew(vector<struct duration_container>& durationRecords,pqxx::result::const_iterator cur_it){
	struct duration_container new_dur;
	new_dur.device_id = cur_it[0].as<int>();
	new_dur.label = cur_it[3].as<string>();
	new_dur.from = cur_it[2].as<string>();
	new_dur.to = "NA";
	new_dur.deauthFlag = false;
	durationRecords.push_back(new_dur);	
}
std::ostream &operator<<(std::ostream &os, duration_container const &obj) { 
    return os << obj.device_id << " "
              << obj.label << " "
              << obj.from << " "
	      << obj.to << " ";
}

}  // namespace ourapi

#endif
