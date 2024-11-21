/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "oauth.h"

bool_t
xdr_ErrorCode (XDR *xdrs, ErrorCode *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_AuthRequest (XDR *xdrs, AuthRequest *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->user_id, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_AuthResponse (XDR *xdrs, AuthResponse *objp)
{
	register int32_t *buf;

	 if (!xdr_ErrorCode (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->auth_token, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_AccessRequest (XDR *xdrs, AccessRequest *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->user_id, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->auth_token, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->refresh))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_AccessResponse (XDR *xdrs, AccessResponse *objp)
{
	register int32_t *buf;

	 if (!xdr_ErrorCode (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->access_token, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->refresh_token, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ttl))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ActionRequest (XDR *xdrs, ActionRequest *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->user_id, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->action, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->resource, ~0))
		 return FALSE;
	return TRUE;
}
