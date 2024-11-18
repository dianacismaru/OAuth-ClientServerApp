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

using namespace std;

class ServerData {
public:
    unordered_set<string> users;
    unordered_set<string> resources;
    deque<unordered_map<string, string>> approvals;

    void loadUsers(const string& filename);
    void loadResources(const string& filename);
    void loadApprovals(const string& filename);
};

extern ServerData server_data;


#endif // SERVER_DATA_H