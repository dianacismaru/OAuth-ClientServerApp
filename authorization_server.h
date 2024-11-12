#ifndef AUTHORIZATION_SERVER_H
#define AUTHORIZATION_SERVER_H

#include "database.h"
#include <string>
#include <unordered_map>

class AuthorizationServer {
private:
    UserDatabase& userDb;
    int maxValidity;

    unordered_map<string, char*> authorizationTokens;
    unordered_map<string, tuple<char*, int>> accessTokens;
    unordered_map<string, char*> refreshTokens;
    unordered_map<string, unordered_map<string, string>> userApprovals;
    
public:
    AuthorizationServer(UserDatabase& userDb, int maxValidity): 
        userDb(userDb), maxValidity(maxValidity) {}
    
    void decreaseValidity(string userId);
    char* getAuthorizationToken(string userId);
    tuple<char*, int> getAccessToken(string userId);
    char* getRefreshToken(string userId);
    unordered_map<string, string> getApprovals(string userId);

    tuple<ErrorCode, char*> requestAuthorization(string userId);
    tuple<ErrorCode, char*> requestAccessToken(string userId, ApprovalDatabase& approvalDb);
    tuple<ErrorCode, char*> requestRefreshToken(string userId);
};

#endif
