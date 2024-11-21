/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "oauth.h"
#include "token.h"
#include <stdio.h>
#include "server_data.h"

AuthResponse *
request_authorization_1_svc(AuthRequest *argp, struct svc_req *rqstp)
{
	static AuthResponse result;
    memset(&result, 0, sizeof(result));
    printf("BEGIN %s AUTHZ\n", argp->user_id);
	fflush(stdout);

	// Check if user exists in the server's database
	if (server_data.users.find(argp->user_id) == server_data.users.end()) {
		result.status = USER_NOT_FOUND;
		result.auth_token = (char*)malloc(1);
		if (!result.auth_token) {
			fprintf(stderr, "[ERR] Server failed to allocate memory for auth token\n");
			exit(1);
		}
		
		result.auth_token[0] = '\0';
		return &result;
	}

	// Generate authorization token
	result.auth_token = generate_access_token(argp->user_id);
	if (!result.auth_token) {
		fprintf(stderr, "[ERR] Server failed to allocate memory for auth_token\n");
		exit(1);
	}

	// Save the authorization token in the server's database
	server_data.users_data[argp->user_id].auth_token = result.auth_token;
	printf("  RequestToken = %s\n", result.auth_token);
	fflush(stdout);
	result.status = NONE;
    return &result;
}

AccessResponse *
request_access_token_1_svc(AccessRequest *argp, struct svc_req *rqstp)
{
	static AccessResponse  result;
	memset(&result, 0, sizeof(result));

	// Check if user has approved the request
	if (!server_data.should_grant_access(argp->user_id)) {
		result.status = REQUEST_DENIED;
		result.ttl = -1;
		result.access_token = (char*)malloc(1);
		result.refresh_token = (char*)malloc(1);

		if (!result.access_token || !result.refresh_token) {
			fprintf(stderr, "[ERR] Server failed to allocate memory for access_token\n");
			exit(1);
		}

		result.access_token[0] = '\0';
		result.refresh_token[0] = '\0';
		return &result;
	}

	result.access_token = generate_access_token(argp->auth_token);
	if (!result.access_token) {
		fprintf(stderr, "[ERR] Server failed to allocate memory for auth_token\n");
		exit(1);
	}

	server_data.users_data[argp->user_id].access_token = result.access_token;
	server_data.users_data[argp->user_id].ttl = server_data.max_ttl;
	printf("  AccessToken = %s\n", result.access_token);
	fflush(stdout);

	result.ttl = server_data.max_ttl;
	result.status = NONE;

	if (!argp->refresh) {
		result.refresh_token = (char*)malloc(1);
		result.refresh_token[0] = '\0';
	} else {
		// Generate refresh token
		result.refresh_token = generate_access_token(result.access_token);
		if (!result.refresh_token) {
			fprintf(stderr, "[ERR] Server failed to allocate memory for refresh_token\n");
			exit(1);
		}

		server_data.users_data[argp->user_id].refresh_token = result.refresh_token;
		printf("  RefreshToken = %s\n", result.refresh_token);
		fflush(stdout);
	}

	return &result;
}

ErrorCode *
validate_action_1_svc(ActionRequest *argp, struct svc_req *rqstp)
{
	static ErrorCode  result;
	memset(&result, 0, sizeof(result));

	char* access_token = server_data.get_access_token(argp->user_id);
	int ttl = server_data.get_ttl(argp->user_id);

	// Check if user has an access token
	if (!access_token || !strcmp(access_token, "")) {
		result = PERMISSION_DENIED;
		printf("DENY (%s,%s,,%d)\n", argp->action, argp->resource, ttl);
		fflush(stdout);
		return &result;
	}

	// Check if token has expired
	if (ttl == 0) {
		// Check if user has a refresh token
		char *refresh_token = server_data.get_refresh_token(argp->user_id);
		if (refresh_token && strcmp(refresh_token, "")) {
			// Send a request to refresh the token
			result = SHOULD_REFRESH;
			return &result;
		}

		// If no refresh token, deny the request
		result = TOKEN_EXPIRED;
		printf("DENY (%s,%s,,%d)\n", argp->action, argp->resource, ttl);
		fflush(stdout);
		return &result;
	}

	// Decrement TTL
	ttl--;
	server_data.users_data[argp->user_id].ttl = ttl;

	// Check if resource exists
	if (server_data.resources.find(argp->resource) == server_data.resources.end()) {
		result = RESOURCE_NOT_FOUND;
		printf("DENY (%s,%s,%s,%d)\n", argp->action, argp->resource, access_token, ttl);
		fflush(stdout);
		return &result;
	}

	// Check if action is permitted on the resource
	if (!server_data.is_action_permitted(argp)) {
		result = OPERATION_NOT_PERMITTED;
		printf("DENY (%s,%s,%s,%d)\n", argp->action, argp->resource, access_token, ttl);
		fflush(stdout);
		return &result;
	}

	// Grant permission
	printf("PERMIT (%s,%s,%s,%d)\n", argp->action, argp->resource, access_token, ttl);
	fflush(stdout);
	result = PERMISSION_GRANTED;
	return &result;
}


ErrorCode *
refresh_tokens_1_svc(AuthRequest *argp, struct svc_req *rqstp)
{
	static ErrorCode  result;
	memset(&result, 0, sizeof(result));
	printf("BEGIN %s AUTHZ REFRESH\n", argp->user_id);

	char *new_access_token = generate_access_token(server_data.users_data[argp->user_id].refresh_token);
	if (!new_access_token) {
		fprintf(stderr, "[ERR] Server failed to allocate memory for access_token\n");
		exit(1);
	}

	server_data.users_data[argp->user_id].access_token = new_access_token;
	server_data.users_data[argp->user_id].ttl = server_data.max_ttl;
	printf("  AccessToken = %s\n", new_access_token);
	fflush(stdout);

	// Generate refresh token
	char *new_refresh_token = generate_access_token(new_access_token);
	if (!new_refresh_token) {
		fprintf(stderr, "[ERR] Server failed to allocate memory for refresh_token\n");
		exit(1);
	}

	server_data.users_data[argp->user_id].refresh_token = new_refresh_token;
	printf("  RefreshToken = %s\n", new_refresh_token);
	fflush(stdout);
	result = NONE;
	return &result;
}
