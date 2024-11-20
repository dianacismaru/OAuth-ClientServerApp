#include "server_data.h"

void ServerData::loadUsers(const string& filename) {
    ifstream file(filename);
    int userCount;
    file >> userCount;
    for (int i = 0; i < userCount; i++) {
        string userId;
        file >> userId;
        users.insert(userId);
    }
    file.close();
}

void ServerData::loadResources(const string& filename) {
    ifstream file(filename);
    int resourceCount;
    file >> resourceCount;
    for (int i = 0; i < resourceCount; i++) {
        string resourceName;
        file >> resourceName;
        resources.insert(resourceName);
    }
    file.close();
}

void ServerData::loadApprovals(const string& filename) {
    ifstream file(filename);
    string line;

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

    file.close();
}

void ServerData::setMaxValidity(int maxValidity) {
    this->maxValidity = maxValidity;
}

bool ServerData::shouldGivePermissions(const string& userId) {
    unordered_map<string, string> approval = approvals.front();
    approvals.pop_front();
    server_data.usersData[userId].approvals = approval; 

    // cout << "Approvals for " << userId << endl;
    // for (const auto& [key, value] : approval) {
    //     cout << key << " -> " << value << endl;
    // }

    if (approval.find("*") != approval.end()) {
        return false;
    }

    return true;
}

char* ServerData::getAccessToken(const string& userId) {
    return server_data.usersData[userId].accessToken;
}

int ServerData::getTtl(const string& userId) {
    return server_data.usersData[userId].ttl;
}

bool ServerData::isActionPermitted(ActionRequest* argp) {
    auto approvals = server_data.usersData[argp->user_id].approvals;
    char actionCode = strcmp(argp->action, "EXECUTE") ? argp->action[0] : 'X' ;

    // Get permissions for the resource
    auto result = approvals.find(argp->resource);
    if (result != approvals.end()) {
        // Check if the action is permitted
        return result->second.find(actionCode) != string::npos;
    }
    
    return false;
}