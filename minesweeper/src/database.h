#pragma once
#include <string>
#include <vector>
#include <mysql/mysql.h>

struct LeaderboardEntry {
    int id;
    std::string player_name;
    int time_seconds;
    std::string created_at;
};

class Database {
public:
    Database(const std::string& host, const std::string& user,
             const std::string& password, const std::string& db_name, int port = 3306);
    ~Database();
    bool init();
    std::vector<LeaderboardEntry> getLeaderboard(int limit = 20, const std::string& difficulty = "easy");
    bool addEntry(const std::string& player_name, int time_seconds, const std::string& difficulty = "easy");
private:
    MYSQL* conn_;
    std::string host_, user_, password_, db_name_;
    int port_;
};
