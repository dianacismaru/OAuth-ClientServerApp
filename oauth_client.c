/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "oauth.h"
#include "client_data.h"

void
oauth_prog_1(char *host, char *filename)
{
	CLIENT *clnt;
	AuthResponse  *result_1;
	AuthRequest  request_authorization_1_arg;
	AccessResponse  *result_2;
	AccessRequest  request_access_token_1_arg;
	ErrorCode  *result_3;
	ActionRequest  validate_action_1_arg;
	ErrorCode  *result_4;
	AuthRequest  refresh_tokens_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, OAUTH_PROG, OAUTH_VERS, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	ClientData client_data;
    client_data.load_requests(filename);

	// Process each request
	for (const auto &request : client_data.requests) {
		string userId = get<0>(request);
        string action = get<1>(request);
        string resource = get<2>(request);

		if (action == "REQUEST") {
			// Request authorization token
			memset(&request_authorization_1_arg, 0, sizeof(request_authorization_1_arg));
			request_authorization_1_arg.user_id = strdup(userId.c_str());
			result_1 = request_authorization_1(&request_authorization_1_arg, clnt);

			if (result_1 == (AuthResponse *) NULL) {
				clnt_perror (clnt, "authz call failed");
				exit(1);
			}

			client_data.handle_error(result_1->status);
			if (result_1->status != ErrorCode::NONE) {
				free(result_1->auth_token);
				continue;
			}

			// Request access token and refresh token if needed
			memset(&request_access_token_1_arg, 0, sizeof(request_access_token_1_arg));
			request_access_token_1_arg.user_id = strdup(userId.c_str());
			request_access_token_1_arg.auth_token = strdup(result_1->auth_token);
			request_access_token_1_arg.refresh = atoi(resource.c_str());

			result_2 = request_access_token_1(&request_access_token_1_arg, clnt);
			if (result_2 == (AccessResponse *) NULL) {
				clnt_perror (clnt, "access call failed");
				exit(1);
			} 

			client_data.handle_error(result_2->status);
			if (result_2->status != ErrorCode::NONE) {
				free(result_1->auth_token);
				free(result_2->access_token);
				free(result_2->refresh_token);
				continue;
			}

			if (resource == "0") {
				printf("%s -> %s\n", result_1->auth_token, result_2->access_token);
			} else {
				printf("%s -> %s,%s\n", result_1->auth_token, result_2->access_token, result_2->refresh_token);
			}
			fflush(stdout);

			free(result_1->auth_token);
			free(result_2->access_token);
			free(result_2->refresh_token);

		} else {
			// Validate action
			memset(&validate_action_1_arg, 0, sizeof(validate_action_1_arg));
			validate_action_1_arg.user_id = strdup(userId.c_str());
			validate_action_1_arg.action = strdup(action.c_str());
			validate_action_1_arg.resource = strdup(resource.c_str());

			result_3 = validate_action_1(&validate_action_1_arg, clnt);
			if (result_3 == (ErrorCode *) NULL) {
				clnt_perror (clnt, "validate action call failed");
				exit(1);
			} 

			if (*result_3 != ErrorCode::SHOULD_REFRESH) {
				client_data.handle_error(*result_3);
				continue;
			}

			// Refresh tokens and validate the action again 
			memset(&refresh_tokens_1_arg, 0, sizeof(refresh_tokens_1_arg));
			refresh_tokens_1_arg.user_id = strdup(userId.c_str());
			result_4 = refresh_tokens_1(&refresh_tokens_1_arg, clnt);

			if (result_4 == (ErrorCode *) NULL) {
				clnt_perror (clnt, "refresh token call failed");
				exit(1);
			} 

			client_data.handle_error(*result_4);
			if (*result_4 == ErrorCode::NONE) {
				result_3 = validate_action_1(&validate_action_1_arg, clnt);
				if (result_3 == (ErrorCode *) NULL) {
					clnt_perror (clnt, "validate action call failed");
					exit(1);
				}
				client_data.handle_error(*result_3);
			}
		}
    }

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	oauth_prog_1 (host, argv[2]);
exit (0);
}
