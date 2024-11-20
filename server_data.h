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
    char* authorizationToken;
    char* accessToken;
    char* refreshToken;
    int ttl;
    unordered_map<string, string> approvals;
};


class ServerData {
public:
    unordered_set<string> users;
    unordered_set<string> resources;
    deque<unordered_map<string, string>> approvals;
    int maxValidity;

    unordered_map<string, UserData> usersData;

    void loadUsers(const string& filename);
    void loadResources(const string& filename);
    void loadApprovals(const string& filename);
    void setMaxValidity(int maxValidity);
    bool shouldGivePermissions(const string& userId);
    char* getAccessToken(const string& userId);
    int getTtl(const string& userId);
    bool isActionPermitted(ActionRequest* argp);
};

extern ServerData server_data;


#endif // SERVER_DATA_H