#include <iostream>
#include <iomanip>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class ReadingProgress{
    enum class Commands: size_t{
        READ  = 4,
        CHEER = 5,
        WRONG = 0
    };

public:
    ReadingProgress()
    :pages_max_number_(1000),
     pages_((pages_max_number_+1), 0)
    {}

    double GetProportionOfStragglers(int user_id);
    void ToRead(int user_id, int page);
    void RunCommand(std::ostream& out, Commands command);
    Commands ParseQuery(const string& query);

private:
    const int pages_max_number_;
    unordered_map<int,int> users_;
    vector<size_t> pages_;

    int user_id_;
    int page_;
};

double 
ReadingProgress::GetProportionOfStragglers(int user_id){
    if(users_.count(user_id_) == 0){
        return 0;
    }

    if(users_.size() == 1){
        return 1;
    }

    size_t stragglers_number = users_.size() - pages_.at(users_.at(user_id));
    return stragglers_number / static_cast<double>(users_.size()-1);
}

void 
ReadingProgress::ToRead(int user_id, int page){
    auto p_begin = pages_.begin() + users_[user_id];
    auto p_end = pages_.begin() + page;

    while(p_begin != p_end){
        ++p_begin;
        *p_begin += 1;
    }

    users_[user_id] = page;
}

void 
ReadingProgress::RunCommand(std::ostream& out, Commands command){
    switch(command){
        case Commands::CHEER: 
            out << setprecision(6) << GetProportionOfStragglers(user_id_) << endl;
        break;
        case Commands::READ:
            ToRead(user_id_, page_);
        break;
        case Commands::WRONG:
            out << "Wrong command" << endl;
        break;
        default:
        break;
    }
}

ReadingProgress::Commands 
ReadingProgress::ParseQuery(const string& query){
    Commands command_length = static_cast<Commands>(query.find(' '));

    switch(command_length){
        case Commands::CHEER:{
            const string& params = query.substr(static_cast<size_t>(Commands::CHEER)+1, query.size());
            user_id_ = stoi(params);

            return Commands::CHEER;
        }
        case Commands::READ:{
            const string& params = query.substr(static_cast<size_t>(Commands::READ)+1, query.size());
            size_t user_length = params.find(' ');

            const string& user_id_str = params.substr(0, user_length);
            user_id_ = stoi(user_id_str);

            const string& page_str = params.substr(user_length+1, params.size());
            page_ = stoi(page_str);

            return Commands::READ;
        }break;
        default:
            return Commands::WRONG;
    }
};

int main() {
    ReadingProgress progress;
    int query_number;
    string query_str;

    cin >> query_number;
    getline(cin, query_str);

    for(int q=0; q<query_number; ++q){
        getline(cin, query_str);
        progress.RunCommand(cout, progress.ParseQuery(query_str));
    }

    return 0;
}