# OAuth

[TOC]

The library implements the OAuth Authorization Code Flow.

When calling `GetAuthenticationInfo`, you supply a set of scopes to request from the user.
In return, you get a URL as well a code. Display both to the user.
The user visits the URL and enters the code.
After logging in to their Twitch account, they are presented with the list of scopes that you are requesting.
If they choose to grant access, the authorization succeeds.
If they choose to decline or simply don't complete the process, the authorization fails.

## Scopes

Refer to https://dev.twitch.tv/docs/authentication#scopes for a list of available OAuth scopes.
Whenever API calls require specific scopes, the documentation text mentions them.

**Requesting no scopes is currently not supported and causes an error.**

## Adding Scopes

The user can only choose to either grant all requested scopes or deny logging in entirely.
After logging in, you can thus expect calls that require scopes which you have requested to succeed.

The only exception to this is a scenario where you expand the set of required scopes in a game update.
In this case, users who logged in with the previous version of the game will remain logged in,
but calls that require a newly added scope will fail.

You can detect this by calling @ref R66::IR66ApiCommon::GetAuthState and checking the value of @ref R66::AuthState::Scopes.

You can resolve this by logging out (@ref R66::IR66ApiCommon::LogOut) and then starting over with 
a fresh call to @ref R66::IR66ApiCommon::GetAuthenticationInfo.

## User-defined Scopes

If you would like give the user some control over what scopes to grant, you can do so by asking them
in your game UI before calling `GetAuthenticationInfo`.
Then, only request scopes that the user has agreed to.
