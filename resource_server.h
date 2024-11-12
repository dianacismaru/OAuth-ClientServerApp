#ifndef RESOURCE_SERVER_H
#define RESOURCE_SERVER_H

#include "database.h"
#include "authorization_server.h"
#include <string>

class ResourceServer {
private:
    ResourceDatabase& resourceDb;
    AuthorizationServer& authServer;

public:
    ResourceServer(ResourceDatabase& resourceDb, AuthorizationServer& authServer)
        : resourceDb(resourceDb), authServer(authServer) {}

    ErrorCode validateAction(const string& userId, const string& action,
                               const string& resource);
};

#endif
