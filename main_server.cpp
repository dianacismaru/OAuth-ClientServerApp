#include "authorization_server.h"
#include "database.h"
#include "resource_server.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: ./server <userIDs.db> <resources.db> <approvals.db> <client.in>" << endl;
        return 1;
    }

    UserDatabase userDb;
    ResourceDatabase resourceDb;
    ApprovalDatabase approvalDb;
    ClientRequests clientRequests;

    loadUserDatabase(userDb, argv[1]);
    loadResourceDatabase(resourceDb, argv[2]);
    loadApprovalDatabase(approvalDb, argv[3]);
    loadClientRequests(clientRequests, argv[4]);
    int maxValidity = 2;

    AuthorizationServer authServer(userDb, maxValidity);
    ResourceServer resourceServer(resourceDb, authServer);

    for (const auto& request : clientRequests.requests) {
        string userId = get<0>(request);
        string action = get<1>(request);
        string resource = get<2>(request);
        clog << "\n[CLIENT.IN] Processing request for " << userId << " to " << action << " resource " << resource << endl;

        try {
            if (action == "REQUEST") {
                auto [authError, authorizationToken] = authServer.requestAuthorization(userId);

                if (authError != ErrorCode::NONE) {
                    handleError(authError);
                    continue;
                }

                auto [accessError, accessToken] = authServer.requestAccessToken(userId, approvalDb);
                if (accessError != ErrorCode::NONE || accessError != ErrorCode::PERMISSION_GRANTED) {
                    handleError(accessError);
                    continue;
                }
                
                if (resource == "1") {
                    auto [refreshError, refreshToken] = authServer.requestRefreshToken(userId);
                }
            } else {
                // make action
                // decrease availability
                auto err = resourceServer.validateAction(userId, action, resource);
                handleError(err);
            }
        } catch (const exception& e) {
            cerr << "Error processing request for " << userId << ": " << e.what() << endl;
        }
    }

    return 0;
}
