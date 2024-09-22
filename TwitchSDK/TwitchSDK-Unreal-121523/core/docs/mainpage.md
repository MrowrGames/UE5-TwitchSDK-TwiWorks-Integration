# Twitch Route 66 {#mainpage}

Welcome to the Twitch Route 66 documentation.
This documentation focuses on the [Core Library](corelib-basics.md).

There are guides for getting started with [Unity](unity.md) and [Unreal Engine](unreal.md).

For either Engine, you must set up the [EventSubProxy](eventsubproxy.md) first. 

There is also reference-level documentation at [R66Api](@ref R66.R66Api) and [IR66ApiCommon](@ref R66.IR66ApiCommon).

## Platform Support

As of right now, the Unreal Engine plugin supports the Win32, Win64, Android and Linux platforms.
For other platforms (e.g. iOS, Mac) we don't anticipate any issues, but no testing was done so far.
Please report any issues that you encounter.

The Unity plugin currently includes core library binaries for Win32 and Win64.
Adding support for other platforms is straightforward:

1. Compile the core library for the desired platform using cmake
2. Drop the binary into your Unity project
3. Unity should automatically recognize the file as a binary for the respective platform. The plugin will then work normally.
