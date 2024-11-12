#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <deque>

using namespace std;

enum class ErrorCode {
    NONE,
    USER_NOT_FOUND,
    RESOURCE_NOT_FOUND,
    PERMISSION_GRANTED,
    OPERATION_NOT_PERMITTED,
    PERMISSION_DENIED,
    REQUEST_DENIED,
    TOKEN_EXPIRED
};

struct UserDatabase {
    unordered_set<string> userIds;
};

struct ResourceDatabase {
    vector<string> resources;
};

struct ApprovalDatabase {
    deque<unordered_map<string, string>> approvals;
};

struct ClientRequests {
    vector<tuple<string, string, string>> requests;
};

void loadUserDatabase(UserDatabase& db, const string& filePath);
void loadResourceDatabase(ResourceDatabase& db, const string& filePath);
void loadApprovalDatabase(ApprovalDatabase& db, const string& filePath);
void loadClientRequests(ClientRequests& requests, const string& filePath);
void handleError(ErrorCode error);

#endif
