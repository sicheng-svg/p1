#include "../include/httplib.h"
#include "database.h"
#include <string>
#include <cstdio>

static std::string extract_json_string(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos + search.size());
    if (pos == std::string::npos) return "";
    pos = json.find('"', pos + 1);
    if (pos == std::string::npos) return "";
    auto end = json.find('"', pos + 1);
    if (end == std::string::npos) return "";
    return json.substr(pos + 1, end - pos - 1);
}

static int extract_json_int(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos + search.size());
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return atoi(json.c_str() + pos);
}

static std::string escape_json_string(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

int main() {
    Database db("localhost", "xsc", "Xsc.200411", "minesweeper");
    if (!db.init()) {
        fprintf(stderr, "Database init failed\n");
        return 1;
    }

    httplib::Server server;

    server.set_mount_point("/", "./static");

    server.Get("/api/leaderboard", [&db](const httplib::Request& req, httplib::Response& res) {
        std::string difficulty = "easy";
        if (req.has_param("difficulty")) {
            difficulty = req.get_param_value("difficulty");
        }
        auto entries = db.getLeaderboard(20, difficulty);
        std::string json = "[";
        for (size_t i = 0; i < entries.size(); i++) {
            if (i > 0) json += ",";
            json += "{\"id\":" + std::to_string(entries[i].id)
                  + ",\"player_name\":\"" + escape_json_string(entries[i].player_name) + "\""
                  + ",\"time_seconds\":" + std::to_string(entries[i].time_seconds)
                  + ",\"created_at\":\"" + escape_json_string(entries[i].created_at) + "\""
                  + "}";
        }
        json += "]";
        res.set_content(json, "application/json");
    });

    server.Post("/api/leaderboard", [&db](const httplib::Request& req, httplib::Response& res) {
        std::string player_name = extract_json_string(req.body, "player_name");
        int time_seconds = extract_json_int(req.body, "time_seconds");
        std::string difficulty = extract_json_string(req.body, "difficulty");
        if (difficulty.empty()) difficulty = "easy";

        if (player_name.empty() || time_seconds <= 0) {
            res.set_content("{\"success\":false,\"error\":\"invalid params\"}", "application/json");
            return;
        }

        bool ok = db.addEntry(player_name, time_seconds, difficulty);
        if (ok) {
            res.set_content("{\"success\":true}", "application/json");
        } else {
            res.set_content("{\"success\":false,\"error\":\"db error\"}", "application/json");
        }
    });

    printf("Server listening on 0.0.0.0:8080\n");
    server.listen("0.0.0.0", 8080);
    return 0;
}
