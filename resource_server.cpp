#include "resource_server.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>

bool isPermittedAction(const string& action, const string& permissions) {
    char actionCode = action == "EXECUTE" ? 'X' : action[0];
    return permissions.find(actionCode) != string::npos;
}

ErrorCode ResourceServer::validateAction(const string& userId, const string& action,
                                           const string& resource) {
    auto [accessToken, validity] = authServer.getAccessToken(userId);
    if (accessToken == nullptr) {
        clog << "[WARN] There is no access token for " << userId << endl;
        clog << "[SERVER.OUT] DENY (" << action << "," << resource << ",," << validity << ")" << endl;
        return ErrorCode::PERMISSION_DENIED;
    }

    if (validity == 0) {
        clog << "[SERVER.OUT] DENY (" << action << "," << resource << ",," << validity << ")" << endl;
        return ErrorCode::TOKEN_EXPIRED;
    }

    clog << "[RES_SERVER] Retrieved access token: " << accessToken << " with validity " << validity << endl;

    authServer.decreaseValidity(userId);

    //check permissions
    auto approval = authServer.getApprovals(userId);
    auto result = approval.find(resource);
    // if the resource not in the resourceDb.resources
    if (find(resourceDb.resources.begin(), resourceDb.resources.end(), resource) == resourceDb.resources.end()) {
        clog << "[SERVER.OUT] DENY (" << action << "," << resource << "," << accessToken << "," << validity - 1 << ")" << endl;
        return ErrorCode::RESOURCE_NOT_FOUND;
    }
    if (result == approval.end() || !isPermittedAction(action, result->second)) {
        clog << "[SERVER.OUT] DENY (" << action << "," << resource << "," << accessToken << "," << validity - 1 << ")" << endl;
        return ErrorCode::OPERATION_NOT_PERMITTED;
    }

    clog << "[SERVER.OUT] PERMIT (" << action << "," << resource << "," << accessToken << "," << validity - 1 << ")" << endl;
    return ErrorCode::PERMISSION_GRANTED;
}

