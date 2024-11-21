/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _OAUTH_H_RPCGEN
#define _OAUTH_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


enum ErrorCode {
	NONE = 0,
	OPERATION_NOT_PERMITTED = 1,
	PERMISSION_GRANTED = 2,
	PERMISSION_DENIED = 3,
	RESOURCE_NOT_FOUND = 4,
	REQUEST_DENIED = 5,
	TOKEN_EXPIRED = 6,
	USER_NOT_FOUND = 7,
};
typedef enum ErrorCode ErrorCode;

struct AuthRequest {
	char *user_id;
};
typedef struct AuthRequest AuthRequest;

struct AuthResponse {
	ErrorCode status;
	char *auth_token;
};
typedef struct AuthResponse AuthResponse;

struct AccessRequest {
	char *user_id;
	char *auth_token;
	int refresh;
};
typedef struct AccessRequest AccessRequest;

struct AccessResponse {
	ErrorCode status;
	char *access_token;
	char *refresh_token;
	int ttl;
};
typedef struct AccessResponse AccessResponse;

struct ActionRequest {
	char *user_id;
	char *action;
	char *resource;
};
typedef struct ActionRequest ActionRequest;

#define OAUTH_PROG 0x20000001
#define OAUTH_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define request_authorization 1
extern  AuthResponse * request_authorization_1(AuthRequest *, CLIENT *);
extern  AuthResponse * request_authorization_1_svc(AuthRequest *, struct svc_req *);
#define request_access_token 2
extern  AccessResponse * request_access_token_1(AccessRequest *, CLIENT *);
extern  AccessResponse * request_access_token_1_svc(AccessRequest *, struct svc_req *);
#define validate_action 3
extern  ErrorCode * validate_action_1(ActionRequest *, CLIENT *);
extern  ErrorCode * validate_action_1_svc(ActionRequest *, struct svc_req *);
extern int oauth_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define request_authorization 1
extern  AuthResponse * request_authorization_1();
extern  AuthResponse * request_authorization_1_svc();
#define request_access_token 2
extern  AccessResponse * request_access_token_1();
extern  AccessResponse * request_access_token_1_svc();
#define validate_action 3
extern  ErrorCode * validate_action_1();
extern  ErrorCode * validate_action_1_svc();
extern int oauth_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_ErrorCode (XDR *, ErrorCode*);
extern  bool_t xdr_AuthRequest (XDR *, AuthRequest*);
extern  bool_t xdr_AuthResponse (XDR *, AuthResponse*);
extern  bool_t xdr_AccessRequest (XDR *, AccessRequest*);
extern  bool_t xdr_AccessResponse (XDR *, AccessResponse*);
extern  bool_t xdr_ActionRequest (XDR *, ActionRequest*);

#else /* K&R C */
extern bool_t xdr_ErrorCode ();
extern bool_t xdr_AuthRequest ();
extern bool_t xdr_AuthResponse ();
extern bool_t xdr_AccessRequest ();
extern bool_t xdr_AccessResponse ();
extern bool_t xdr_ActionRequest ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_OAUTH_H_RPCGEN */
