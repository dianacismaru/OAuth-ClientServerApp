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

struct AuthRequest {
    string user_id<>;
};

struct AuthResponse {
    ErrorCode status;
    string auth_token<>;
};

struct AccessRequest {
    string user_id<>;
    string auth_token<>;
    int refresh;
};

struct AccessResponse {
    ErrorCode status;
    string access_token<>;
    string refresh_token<>;
    int ttl;
};

struct ActionRequest {
    string user_id<>;
    string action<>; /* request sau RWXMI*/
    string resource<>; /* numele resursei sau 0/1 pt refresh*/
};

program OAUTH_PROG {
    version OAUTH_VERS {
        AuthResponse request_authorization(AuthRequest) = 1;
        AccessResponse request_access_token(AccessRequest) = 2;
        ErrorCode validate_action(ActionRequest) = 3;
        ErrorCode refresh_tokens(AuthRequest) = 4;
    } = 1;
} = 0x20000001;