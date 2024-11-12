#include "authorization_server.h"
#include "token.h"
#include <stdexcept>
#include <tuple>
#include <cstring>

char* AuthorizationServer::getAuthorizationToken(string userId) {
    return authorizationTokens[userId];
}

tuple<char*, int> AuthorizationServer::getAccessToken(string userId) {
    return accessTokens[userId];
}

char* AuthorizationServer::getRefreshToken(string userId) {
    return refreshTokens[userId];
}

void AuthorizationServer::decreaseValidity(string userId) {
    get<1>(accessTokens[userId]) -= 1;
}

unordered_map<string, string> AuthorizationServer::getApprovals(string userId) {
    return userApprovals[userId];
}

tuple<ErrorCode, char*> AuthorizationServer::requestAuthorization(string userId) {
    clog << "[SERVER.OUT] BEGIN " << userId << " AUTHZ" << endl;
    if (userDb.userIds.find(userId) == userDb.userIds.end()) {
        return {ErrorCode::USER_NOT_FOUND, ""};
    }

    char* authorizationToken = generate_access_token(const_cast<char*>(userId.c_str()));
    clog << "[SERVER.OUT]  RequestToken = " << authorizationToken << endl;
    authorizationTokens[userId] = authorizationToken;
    return {ErrorCode::NONE, authorizationToken};
}

tuple<ErrorCode, char*> AuthorizationServer::requestAccessToken(string userId, ApprovalDatabase& approvalDb) {
    char* authorizationToken = authorizationTokens[userId];
    unordered_map<string, string> approval = approvalDb.approvals.front();
    approvalDb.approvals.pop_front();
    userApprovals[userId] = approval;
    
    // Request was not approved by the user
    if (approval.find("*") != approval.end()) {
        clog << "[INFO] Request denied for " << userId << endl;
        return {ErrorCode::REQUEST_DENIED, authorizationToken};
    }

    char* accessToken = generate_access_token(authorizationToken);
    accessTokens[userId] = make_tuple(accessToken, maxValidity);
    clog << "[SERVER.OUT]  AccessToken = " << accessToken << endl;
    clog << "[CLIENT.OUT] " << authorizationToken << " -> " << accessToken << endl;
    return {ErrorCode::NONE, accessToken};
}

tuple<ErrorCode, char*> AuthorizationServer::requestRefreshToken(string userId) {
    char* accessToken = get<0>(accessTokens[userId]);
    char* refreshToken = generate_access_token(accessToken);
    refreshTokens[userId] = refreshToken;
    clog << "[SERVER.OUT]  RefreshToken = " << refreshToken << endl;

    return {ErrorCode::NONE, refreshToken};
}