_Name:_ Diana-Iuliana Cismaru \
_Grupa:_ 341C1

# Client/Server Application for OAuth Autorization using RPC

## Summary

This application implements an authorization and resource access system using Remote Procedure Call (RPC). The project consists of a server and a client, communicating through RPC, with the following functionalities:

- **Authorization:** Verifying a user and generating an authentication token.
- **Resource Access:** Generating an access token for a user that approves the request token
- **Action Management:** Validating user rights over resources.

---

## Flow explanation

### `oauth.x`: RPC definition file

- `AuthResponse request_authorization(AuthRequest)`:

  - used by the client to request authorization from the server
  - gets the AuthRequest, which is composed of the user ID
  - returns the AuthResponse, containing the authentication token and an error
    code

- `AccessResponse request_access(AccessRequest)`:

  - used by the client to request access to a resource
  - gets the AccessRequest, which is composed of the authentication token, the
    user ID and the refresh flag
  - returns the AccessResponse, containing the error code, the access token, the
    validity of the access token and the refresh token

- `ErrorCode validate_action(ActionRequest)`:

  - used by the client to validate an action on a resource
  - gets the ActionRequest, which is composed of the access token, the resource
    ID and the action to be validated
  - returns the ErrorCode, indicating the success or failure of the action

- `ErrorCode refresh_tokens(AuthRequest)`:

  - used by the client to refresh the access token when it expires
  - gets the AuthRequest, which is composed of the user ID
  - returns the ErrorCode of the operation

### How it works

#### Server side:

1. The server reads the user IDs, resources and approvals from the input files.
2. The server listens for incoming requests from the client.
3. When a client **requests authorization**, the server verifies the user and
   returns an authentication token if the user exists in the database.
4. When a client **requests access token**, the server validates the
   authentication token if the end-user has signed an approval for the resource
   (will not sign if the approval is equal to `*,-`). It will also generate a
   refresh token if the refresh flag is set at the REQUEST action.
5. When a client requests **validating an action**, the server checks if the
   access token is valid and if the user has the right to perform the action on
   the resource. If the access token is expired, the server will generate new
   tokens using the refresh token before validating the action.

#### Client side:

1. Client reads the requests from the input file and iterates through them.
2. For each request, the client sends the request to the server and waits for
   the response.
3. The client prints the response to the standard output, indicating the
   success or failure of the request, or the set of tokens generated.

### Modifications I made

- I loaded the input data in the object `server_data` in `oauth_svc.c`, so the
  file should **not** be overriden using `rpcgen`.
- I added newlines in the expected output files to match the actual output.
- I created a Makefile to compile the project using g++ and linked `tirpc`
  library.
- In `oauth_client.c`:
  - When calling `clnt_create`, I used the `TCP` protocol instead of `UDP` to
    avoid unusual segmentation fault.
  - I added command-line arguments in the `main` function to read the server
    address and the input file (client.in), and sent them to the `oauth_prog_1`
    function.
- Created `server_data.cpp` to load the server data from the input files and
  store it in memory.
- Added casting to `(char*)` when allocating memory for the token in the
  `generate_token` function to avoid warnings.

## Compilation and execution

```bash
make clean
make -f Makefile.oauth
```

```bash
./oauth_server tests/test<no>/userIDs.db tests/test<no>/resources.db tests/test<no>/approvals.db <max_validity>
```

```bash
./oauth_client localhost tests/test<no>/client.in
```

## Run the tests

```bash
make clean
make -f Makefile.oauth
./check.sh all
```
