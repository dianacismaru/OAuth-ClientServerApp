#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <string>

#include "oauth.h"

using namespace std;

class ClientData {
public:
    // Tuple of (user_id, action, resource) from a request
    vector<tuple<string, string, string>> requests;

    void load_requests(const string& filename);
    void handle_error(ErrorCode error);
};


#endif // CLIENT_DATA_H