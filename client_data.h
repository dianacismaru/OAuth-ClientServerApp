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
    vector<tuple<string, string, string>> requests;

    void loadRequests(const string& filename);
    void handleError(ErrorCode error);
};


#endif // CLIENT_DATA_H