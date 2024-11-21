#include "server_data.h"

// Load users from a file
void ServerData::load_users(const string& filename) {
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

// Load resources from a file
void ServerData::load_resources(const string& filename) {
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

// Load approvals from a file
void ServerData::load_approvals(const string& filename) {
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

// Set the maximum time-to-live for a token
void ServerData::set_max_ttl(int max_ttl) {
    this->max_ttl = max_ttl;
}

// Check if access should be granted to a user by checking
// the next approval in the queue
bool ServerData::should_grant_access(const string& userId) {
    unordered_map<string, string> approval = approvals.front();
    approvals.pop_front();
    server_data.users_data[userId].approvals = approval; 

    if (approval.find("*") != approval.end()) {
        return false;
    }

    return true;
}

// Check if an action is permitted for a user
bool ServerData::is_action_permitted(ActionRequest* argp) {
    auto approvals = server_data.users_data[argp->user_id].approvals;
    char actionCode = strcmp(argp->action, "EXECUTE") ? argp->action[0] : 'X' ;

    // Get permissions for the resource
    auto result = approvals.find(argp->resource);
    if (result != approvals.end()) {
        // Check if the action is permitted
        return result->second.find(actionCode) != string::npos;
    }
    
    return false;
}

// Retrieve the access token for a user
char* ServerData::get_access_token(const string& userId) {
    return server_data.users_data[userId].access_token;
}

// Retrieve the refresh token for a user
char* ServerData::get_refresh_token(const string& userId) {
    return server_data.users_data[userId].refresh_token;
}

// Retrieve the time-to-live for a user's access token
int ServerData::get_ttl(const string& userId) {
    return server_data.users_data[userId].ttl;
}
