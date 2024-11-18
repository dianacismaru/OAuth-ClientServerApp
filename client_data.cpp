#include "client_data.h"

void ClientData::loadRequests(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string userId, action, resource;
        getline(ss, userId, ',');
        getline(ss, action, ',');
        getline(ss, resource, ',');
        requests.emplace_back(userId, action, resource);
    }
    clog << "[INFO] Finished loading requests db" << endl;

    file.close();
}

// void handleError(ErrorCode error) {
//     switch (error) {
//         case ErrorCode::USER_NOT_FOUND:
//             clog << "[CLIENT.OUT] USER_NOT_FOUND" << endl;
//             break;
//         case ErrorCode::RESOURCE_NOT_FOUND:
//             clog << "[CLIENT.OUT] RESOURCE_NOT_FOUND" << endl;
//             break;
//         case ErrorCode::PERMISSION_GRANTED:
//             clog << "[CLIENT.OUT] PERMISSION_GRANTED" << endl;
//             break;
//         case ErrorCode::OPERATION_NOT_PERMITTED:
//             clog << "[CLIENT.OUT] OPERATION_NOT_PERMITTED" << endl;
//             break;
//         case ErrorCode::REQUEST_DENIED:
//             clog << "[CLIENT.OUT] REQUEST_DENIED" << endl;
//             break;
//         case ErrorCode::PERMISSION_DENIED:
//             clog << "[CLIENT.OUT] PERMISSION_DENIED" << endl;
//             break;
//         case ErrorCode::TOKEN_EXPIRED:
//             clog << "[CLIENT.OUT] TOKEN_EXPIRED" << endl;
//             break;
//         case ErrorCode::NONE:
//             break;
//         default:
//             clog << "[WARN] UNKNOWN ERROR" << endl;
//             break;
//     }
// }

