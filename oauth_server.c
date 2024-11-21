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

	if (server_data.users.find(argp->user_id) == server_data.users.end()) {
		result.status = USER_NOT_FOUND;
		result.auth_token = (char*)malloc(1);
		if (result.auth_token != NULL) {
			result.auth_token[0] = '\0';
		} else {
			fprintf(stderr, "[ERR] Server failed to allocate memory for auth token\n");
		}
		return &result;
	}

	result.status = NONE;
	result.auth_token = generate_access_token(argp->user_id);
	if (result.auth_token) {
		printf("  RequestToken = %s\n", result.auth_token);
		fflush(stdout);
		server_data.usersData[argp->user_id].authorizationToken = result.auth_token;
	} else {
		fprintf(stderr, "[ERR] Server failed to allocate memory for auth_token\n");
	}
    return &result;
}

// ar trebui sa trimit si refresh flag si sa generez automat si cel de refresh
AccessResponse *
request_access_token_1_svc(AccessRequest *argp, struct svc_req *rqstp)
{
	static AccessResponse  result;
	memset(&result, 0, sizeof(result));
	printf("refresh = %d\n", argp->refresh);

	char* authorizationToken = server_data.usersData[argp->user_id].authorizationToken;
	if (!server_data.shouldGivePermissions(argp->user_id)) {
		result.status = REQUEST_DENIED;
		result.access_token = (char*)malloc(1);
		result.refresh_token = (char*)malloc(1);
		result.ttl = -1;
		if (result.access_token != NULL) {
			result.access_token[0] = '\0';
			result.refresh_token[0] = '\0';
		} else {
			fprintf(stderr, "[ERR] Server failed to allocate memory for access_token\n");
		}
		return &result;
	}

	result.status = NONE;
	result.access_token = generate_access_token(argp->auth_token);
	if (result.access_token) {
		printf("  AccessToken = %s\n", result.access_token);
		fflush(stdout);
		server_data.usersData[argp->user_id].accessToken = result.access_token;
		server_data.usersData[argp->user_id].ttl = server_data.maxValidity;
	} else {
		fprintf(stderr, "[ERR] Server failed to allocate memory for auth_token\n");
	}
	
	// TODO continue with refresh token
	if (true) {
		result.refresh_token = (char*)malloc(1);
		result.refresh_token[0] = '\0';
		result.ttl = server_data.maxValidity;
	}

	return &result;
}

ErrorCode *
validate_action_1_svc(ActionRequest *argp, struct svc_req *rqstp)
{
	static ErrorCode  result;

	/*
	 * insert server code here
	 */
	memset(&result, 0, sizeof(result));
	char* accessToken = server_data.getAccessToken(argp->user_id);
	int ttl = server_data.getTtl(argp->user_id);

	if (accessToken == NULL || !strcmp(accessToken, "")) {
		result = PERMISSION_DENIED;
		printf("DENY (%s,%s,,%d)\n", argp->action, argp->resource, ttl);
		fflush(stdout);
		return &result;
	}

	if (ttl == 0) {
		result = TOKEN_EXPIRED;
		printf("DENY (%s,%s,,%d)\n", argp->action, argp->resource, ttl);
		fflush(stdout);
		return &result;
	}

	ttl--;
	server_data.usersData[argp->user_id].ttl = ttl;

	if (server_data.resources.find(argp->resource) == server_data.resources.end()) {
		result = RESOURCE_NOT_FOUND;
		printf("DENY (%s,%s,%s,%d)\n", argp->action, argp->resource, accessToken, ttl);
		fflush(stdout);
		return &result;
	}

	if (!server_data.isActionPermitted(argp)) {
		result = OPERATION_NOT_PERMITTED;
		printf("DENY (%s,%s,%s,%d)\n", argp->action, argp->resource, accessToken, ttl);
		fflush(stdout);
		return &result;
	}

	printf("PERMIT (%s,%s,%s,%d)\n", argp->action, argp->resource, accessToken, ttl);
	fflush(stdout);
	result = PERMISSION_GRANTED;
	return &result;
}
