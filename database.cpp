#include "database.h"

void loadUserDatabase(UserDatabase& db, const string& filePath) {
    clog << "[INFO] Start loading user db" << endl;
    ifstream file(filePath);
    int userCount;
    file >> userCount;
    for (int i = 0; i < userCount; i++) {
        string userId;
        file >> userId;
        db.userIds.insert(userId);
    }
    clog << "[INFO] Finished loading user db" << endl;
    file.close();
}

void loadResourceDatabase(ResourceDatabase& db, const string& filePath) {
    clog << "[INFO] Start loading resource db" << endl;
    ifstream file(filePath);
    int resourceCount;
    file >> resourceCount;
    for (int i = 0; i < resourceCount; i++) {
        string resourceName;
        file >> resourceName;
        db.resources.push_back(resourceName);
    }
    clog << "[INFO] Finished loading resource db" << endl;
    file.close();
}

void loadApprovalDatabase(ApprovalDatabase& db, const string& filePath) {
    ifstream file(filePath);
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

        db.approvals.push_back(hashmap);
    }
    clog << "[INFO] Finished loading approvals db" << endl;

    file.close();
}

void loadClientRequests(ClientRequests& requests, const string& filePath) {
    ifstream file(filePath);
    string line;
    clog << "[INFO] Start loading requests db" << endl;
    while (getline(file, line)) {
        stringstream ss(line);
        string userId, action, resource;
        getline(ss, userId, ',');
        getline(ss, action, ',');
        getline(ss, resource, ',');
        requests.requests.emplace_back(userId, action, resource);
    }
    clog << "[INFO] Finished loading requests db" << endl;
    file.close();
}

void handleError(ErrorCode error) {
    switch (error) {
        case ErrorCode::USER_NOT_FOUND:
            clog << "[CLIENT.OUT] USER_NOT_FOUND" << endl;
            break;
        case ErrorCode::RESOURCE_NOT_FOUND:
            clog << "[CLIENT.OUT] RESOURCE_NOT_FOUND" << endl;
            break;
        case ErrorCode::PERMISSION_GRANTED:
            clog << "[CLIENT.OUT] PERMISSION_GRANTED" << endl;
            break;
        case ErrorCode::OPERATION_NOT_PERMITTED:
            clog << "[CLIENT.OUT] OPERATION_NOT_PERMITTED" << endl;
            break;
        case ErrorCode::REQUEST_DENIED:
            clog << "[CLIENT.OUT] REQUEST_DENIED" << endl;
            break;
        case ErrorCode::PERMISSION_DENIED:
            clog << "[CLIENT.OUT] PERMISSION_DENIED" << endl;
            break;
        case ErrorCode::TOKEN_EXPIRED:
            clog << "[CLIENT.OUT] TOKEN_EXPIRED" << endl;
            break;
        case ErrorCode::NONE:
            break;
        default:
            clog << "[WARN] UNKNOWN ERROR" << endl;
            break;
    }
}
