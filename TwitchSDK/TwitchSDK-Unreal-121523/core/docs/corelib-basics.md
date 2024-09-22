# Core Library

[TOC]

## Basic Organization

Most of the Twitch R66 plugin is implemented in the C++ Core Library.
It is used directly in Unreal Engine, and wrapped in a C# plugin for Unity.

## Promises

The library is designed around a Promise-style callback pattern based on `std::function`.
Most API calls are asynchronous, and do not directly return a value.
Instead, they take a @ref R66::ResolveFn and a @ref R66::RejectFn.

If the call fails, it will either throw an exception immediately or invoke the @ref R66::RejectFn with the exception object as a parameter.
If the call succeeds, it will invoke the @ref R66::ResolveFn and provide the result value as a parameter.

Asynchronous calls which don't return a value take a `ResolveFn<void>`.
The resolve callback does not have any parameter in this case.

## Strings

The Core Library supports the following string types (@ref R66::string):

- `std::string`
- `std::u16string`
- `std::wstring`

At compile time, one of these types is selected. This will be the native string type used internally, as well as for all APIs.
The Unreal and Unity game engine plugins each select a string type appropriate for their environment.
The Unity plugin always picks `u16string` for zero-cost conversion with C# strings.
Unreal Engine's `TCHAR' corresponds to either `u16string` or `wstring` depending on the target platform.
The plugin sets flags for the core library accordingly.

## JSON

The core library bundles the MIT-licensed `nlohmann::json` library by default to serialize and deserialize JSON HTTP bodies.
You can define `R66_JSON_EXTERNAL` to provide your own implementation.

For example, the Unreal Engine plugin does this in order to use Unreal's native JSON implementation instead.
This avoids bundling an unnecessary JSON library.
