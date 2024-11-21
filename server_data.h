#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <deque>
#include "oauth.h"

using namespace std;

struct UserData {
    char* auth_token;
    char* access_token;
    char* refresh_token;
    int ttl; /* Time-to-live for the access token */
    unordered_map<string, string> approvals; /* Current approvals */
};


class ServerData {
public:
    // Set of users in the system
    unordered_set<string> users;

    // Set of resources in the system
    unordered_set<string> resources;

    // Queue of approvals
    deque<unordered_map<string, string>> approvals;

    // Maximum time-to-live for a token
    int max_ttl;

    // Map of user data
    unordered_map<string, UserData> users_data;

    void load_users(const string& filename);
    void load_resources(const string& filename);
    void load_approvals(const string& filename);
    void set_max_ttl(int max_ttl);
    bool should_grant_access(const string& userId);
    bool is_action_permitted(ActionRequest* argp);

    char* get_access_token(const string& userId);
    char* get_refresh_token(const string& userId);
    int get_ttl(const string& userId);
};

extern ServerData server_data;


#endif // SERVER_DATA_H