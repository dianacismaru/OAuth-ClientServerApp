#include "server_data.h"

void ServerData::loadUsers(const string& filename) {
    clog << "[INFO] Start loading user db" << endl;
    ifstream file(filename);
    int userCount;
    file >> userCount;
    for (int i = 0; i < userCount; i++) {
        string userId;
        file >> userId;
        users.insert(userId);
    }
    clog << "[INFO] Finished loading user db" << endl;
    file.close();
}

void ServerData::loadResources(const string& filename) {
    clog << "[INFO] Start loading resource db" << endl;
    ifstream file(filename);
    int resourceCount;
    file >> resourceCount;
    for (int i = 0; i < resourceCount; i++) {
        string resourceName;
        file >> resourceName;
        resources.insert(resourceName);
    }
    clog << "[INFO] Finished loading resource db" << endl;
    file.close();
}

void ServerData::loadApprovals(const string& filename) {
    ifstream file(filename);
    string line;
    clog << "[INFO] Start loading approvals db" << endl;

    while (getline(file, line)) {
        unordered_map<string, string> hashmap;
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        for (size_t i = 0; i < tokens.size(); i += 2) {
            hashmap[tokens[i]] = tokens[i + 1];
        }

        approvals.push_back(hashmap);
    }
    clog << "[INFO] Finished loading approvals db" << endl;

    file.close();
}
