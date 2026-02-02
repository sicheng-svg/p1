#include "database.h"
#include <cstdio>
#include <cstring>

Database::Database(const std::string& host, const std::string& user,
                   const std::string& password, const std::string& db_name, int port)
    : conn_(nullptr), host_(host), user_(user), password_(password), db_name_(db_name), port_(port) {}

Database::~Database() {
    if (conn_) {
        mysql_close(conn_);
        conn_ = nullptr;
    }
}

bool Database::init() {
    conn_ = mysql_init(nullptr);
    if (!conn_) {
        fprintf(stderr, "mysql_init failed\n");
        return false;
    }

    if (!mysql_real_connect(conn_, host_.c_str(), user_.c_str(), password_.c_str(),
                            db_name_.c_str(), port_, nullptr, 0)) {
        fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn_));
        return false;
    }

    const char* create_table =
        "CREATE TABLE IF NOT EXISTS leaderboard ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "player_name VARCHAR(50) NOT NULL, "
        "time_seconds INT NOT NULL, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";

    if (mysql_query(conn_, create_table)) {
        fprintf(stderr, "CREATE TABLE failed: %s\n", mysql_error(conn_));
        return false;
    }

    // Add difficulty column if it doesn't exist
    mysql_query(conn_,
        "ALTER TABLE leaderboard ADD COLUMN difficulty VARCHAR(10) NOT NULL DEFAULT 'easy'");

    return true;
}

std::vector<LeaderboardEntry> Database::getLeaderboard(int limit, const std::string& difficulty) {
    std::vector<LeaderboardEntry> entries;

    char escaped_diff[21];
    mysql_real_escape_string(conn_, escaped_diff, difficulty.c_str(), difficulty.size());

    char query[512];
    snprintf(query, sizeof(query),
             "SELECT id, player_name, time_seconds, created_at FROM leaderboard "
             "WHERE difficulty = '%s' ORDER BY time_seconds ASC LIMIT %d", escaped_diff, limit);

    if (mysql_query(conn_, query)) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn_));
        return entries;
    }

    MYSQL_RES* result = mysql_store_result(conn_);
    if (!result) {
        return entries;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        LeaderboardEntry entry;
        entry.id = row[0] ? atoi(row[0]) : 0;
        entry.player_name = row[1] ? row[1] : "";
        entry.time_seconds = row[2] ? atoi(row[2]) : 0;
        entry.created_at = row[3] ? row[3] : "";
        entries.push_back(entry);
    }

    mysql_free_result(result);
    return entries;
}

bool Database::addEntry(const std::string& player_name, int time_seconds, const std::string& difficulty) {
    char escaped_name[101];
    mysql_real_escape_string(conn_, escaped_name, player_name.c_str(), player_name.size());

    char escaped_diff[21];
    mysql_real_escape_string(conn_, escaped_diff, difficulty.c_str(), difficulty.size());

    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO leaderboard (player_name, time_seconds, difficulty) VALUES ('%s', %d, '%s')",
             escaped_name, time_seconds, escaped_diff);

    if (mysql_query(conn_, query)) {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn_));
        return false;
    }

    return true;
}
