 /* error codes handled by the client */
enum ErrorCode {
    NONE,
    OPERATION_NOT_PERMITTED,
    PERMISSION_GRANTED,
    PERMISSION_DENIED,
    RESOURCE_NOT_FOUND,
    REQUEST_DENIED,
    TOKEN_EXPIRED,
    USER_NOT_FOUND,
    SHOULD_REFRESH
};

/* structure for the request of the authorization token */
struct AuthRequest {
    string user_id<>; /* the user id */
}; 

struct AuthResponse {
    ErrorCode status; /* status of the request */
    string auth_token<>; /* the authorization token */
}; 

struct AccessRequest {
    string user_id<>; /* the user id */
    string auth_token<>; /* the authorization token */
    int refresh; /* flag to indicate if refresh token is needed */
};

struct AccessResponse {
    ErrorCode status; /* status of the request */
    string access_token<>; /* the access token */
    string refresh_token<>; /* the refresh token */
    int ttl; /* time to live of the access token */
};

struct ActionRequest {
    string user_id<>; /* the user id */
    string action<>; /* REQUEST or RIMDX action */
    string resource<>; /* the resource or flag for refresh */
};

program OAUTH_PROG {
    version OAUTH_VERS {
        /* Method to request the authorization token */
        AuthResponse request_authorization(AuthRequest) = 1;

        /* Method to request the access token */
        AccessResponse request_access_token(AccessRequest) = 2;

        /* Method to validate the action */
        ErrorCode validate_action(ActionRequest) = 3;

        /* Method to refresh the tokens */
        ErrorCode refresh_tokens(AuthRequest) = 4;
    } = 1;
} = 0x20000001;