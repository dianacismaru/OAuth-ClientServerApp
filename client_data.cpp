#include "client_data.h"

// Load requests from a file
void ClientData::load_requests(const string& filename) {
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

    file.close();
}

// Handle error codes
void ClientData::handle_error(ErrorCode error) {
    switch (error) {
        case ErrorCode::USER_NOT_FOUND:
            printf("USER_NOT_FOUND\n");
            break;
        case ErrorCode::RESOURCE_NOT_FOUND:
            printf("RESOURCE_NOT_FOUND\n");
            break;
        case ErrorCode::PERMISSION_GRANTED:
            printf("PERMISSION_GRANTED\n");
            break;
        case ErrorCode::OPERATION_NOT_PERMITTED:
            printf("OPERATION_NOT_PERMITTED\n");
            break;
        case ErrorCode::REQUEST_DENIED:
            printf("REQUEST_DENIED\n");
            break;
        case ErrorCode::PERMISSION_DENIED:
            printf("PERMISSION_DENIED\n");
            break;
        case ErrorCode::TOKEN_EXPIRED:
            printf("TOKEN_EXPIRED\n");
            break;
        case ErrorCode::SHOULD_REFRESH:
        case ErrorCode::NONE:
            break;
        default:
            printf("UNKNOWN_ERROR\n");
            break;
    }
    fflush(stdout);
}

