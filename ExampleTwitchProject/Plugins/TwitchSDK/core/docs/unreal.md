# Getting Started: Unreal Engine 4

[TOC]

## Installing

First, close the Unreal Editor. Navigate in Windows Explorer to your project directory. If it doesn't exist yet, create a `Plugins` directory.

![](ue4-plugins-dir.png)

Then, copy the `output` directory into it

![](ue4-copy-plugin.png)

Now, you can restart the Unreal editor, and the plugin should be loaded!

## Configuration

Of you haven't already, head over to https://dev.twitch.com and create an OAuth application.

Then, configure your Client-Id and Secret in the Project Settings.

![](ue4-project-settings.png)

**The Beta version of the library currently requires an OAuth Secret to be specified. This will be changed soon.
Note that you should never publish your OAuth Secret secret.
Hence, you must not make a build of the game publicly available if it contains an OAuth secret.**

See [OAuth](oauth.md) for general notes on authentication.

## Blueprint API

Using the plugin directly from UE4 Blueprints is supported.

### Authentication

Most APIs require the user to be authenticated with a Twitch account.

![](ue4-auth.png)

Use the *Get Authentication Info* node to connect to the Twitch API and generate **Uri** and **User Code**.
Show both values to the user. They need to navigate to the **Uri** (typically https://www.twitch.tv/activate)
with a web browser and type in the **User Code**.

Note that the example above makes use of the *Split Struct Pin* option:

![](ue4-split-pin.png)

### Error Handling

All APIs have exactly four output pins: two execution pins *Done* and *Error* and two data pins *Result* and *Error*.

If a call succeeds, the *Done* pin is called and *Result* is set to the return value.
Make use of the *Split Struct Pin* option as shown above to avoid cluttering your blueprint with many *Break Struct* nodes.
Ignore the value of *Error* in this case.

If a call fails, the *Error* pin is called and *Error* is set to a string describing the error that occurred.
Ignore the value of *Result* in this case.

For convenience, if an operation fails but no action is connected to the *Error* pin, the error is always logged to the console.
Keep in mind that nodes that do have an action connected to their *Error* pin will, conversely, produce no such messages.
This is useful to avoid spamming the console with expected errors, but may be confusing during debugging.

### Reference Documentation

The available blueprint nodes correspond directly to the @ref R66::IR66ApiCommon interface.
For reference-level documentation, head over there.

### Note regarding functions

Since most R66 APIs are asynchronous, Unreal Engine does not allow them to be used in Blueprint Fuctions.
Use Blueprint Macros instead.


## C++ API

Using the plugin directly from C++ is supported.

### Build Configuration

After installing and configuring the plugin, simply reference it from your module's `.Build.cs` file.

```
		PrivateDependencyModuleNames.Add("R66");
```

Note that the plugin requires C++17, so you also need to add the following declaration if you haven't already:

```
		CppStandard = CppStandardVersion.Cpp17;
```

### Using the API

To obtain a reference to the core library, include `R66.h` and then get it from the module.
All asynchronous APIs are based around promise-style *resolve* and *reject* callbacks.
Below is a small example.

```
#include "MyActorComponent.h"
#include "R66.h"

void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();

	auto core = FModuleManager::GetModuleChecked<FR66Module>("R66").Core;
	core->GetAuthenticationInfo(
		R66::BuildOAuthScopes({ FR66OAuthScope::ChannelManageBroadcast, FR66OAuthScope::ChannelManagePolls }),
		[](const R66::AuthenticationInfo& info) {
			if (info.UserCode.size() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("The user is already authenticated"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("The user should authenticate at %s with code %s"), info.Uri.data(), info.UserCode.data());
			}
		},
		[](const std::exception& e) {
			UE_LOG(LogTemp, Warning, TEXT("GetAuthenticationInfo failed: %s"), UTF8_TO_TCHAR(e.what()));
		}
	);
}
```



### Reference Documentation

The core library interface is @ref R66::R66Api.
For reference-level documentation, head over there.
