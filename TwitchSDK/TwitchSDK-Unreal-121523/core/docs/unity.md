# Getting Started: Unity

[TOC]

## Installing the SDK

### Importing the package

Drag and drop the `r66.unitypackage` file into the asset manager in Unity, and import the package.

![Drag and drop the file](unity-drop.png) ![Import all files](unity-import.png)

### Rebuilding the project files

In some cases, you will need to rebuild the project files for Unity and Visual Stuido to properly recognize all the code assets.
For this, go to `Edit -> Preferences` in the Unity Editor menu, and under `External Tools`, press the `Regenerate Project Files` button.

![Regenerate the project files](unity-regenerate.png)

## Setting up the SDK

To set up the SDK, acquire  a client id and secret at [dev.twitch.tv](https://dev.twitch.tv/)

Then, click the `Twitch R66 -> Edit Settings` in the Unity Menu, and fill in the client-id and secret in the "Inspector" window. You should see a notification in the Inspector once your credentials are correct.

![Set up the client-id and secret](unity-settings.png)

**The Beta version of the library currently requires an OAuth Secret to be specified. This will be changed soon.
Note that you should never publish your OAuth Secret secret.
Hence, you must not make a build of the game publicly available if it contains an OAuth secret.**

See [OAuth](oauth.md) for general notes on authentication.

See [GameTask](gametask.md) for detailed information about the `GameTask` type that is used throughout the plugin's API.

## Writing Code with the SDK

Now you can work with the Route66 SDK! Let's create a small test script to see everything in action. 

Create a new script. In the `Start` function, we get the authentication info.
Once we have the authentication info, we print it to the user. 

Every frame, we check if we know who's logged in - and once we do, we welcome them. 

```cs
using R66;
using R66.Interop;
using UnityEngine;

public class WelcomeBehavior : MonoBehaviour
{
    GameTask<AuthenticationInfo> AuthInfoTask;
    bool HasPrintedWelcomeMessage = false;

    void Start()
    {
        AuthInfoTask = Twitch.API.GetAuthenticationInfo(TwitchOAuthScope.Channel.ManageBroadcast);
    }

    // Update is called once per frame
    void Update()
    {
        if (AuthInfoTask?.IsCompleted == true)
        {
            // If the Uri is null, it means the user is already authenticated.
            if (AuthInfoTask.MaybeResult.Uri != null)
            {
                Debug.Log($"Please log in with the code {AuthInfoTask.MaybeResult.UserCode} at the address {AuthInfoTask.MaybeResult.Uri}");

                // We set the AuthInfoTask to null, so the above message isn't printed again and again.
                AuthInfoTask = null;
            }
        }

        // If we already printed the welcome message, there's nothing to do.
        if (HasPrintedWelcomeMessage)
            return;

        var userInfoTask = Twitch.API.GetMyUserInfo();
        if (userInfoTask.IsCompleted)
        {
            Debug.Log($"We are logged in as: {userInfoTask.MaybeResult.DisplayName} - welcome!");
            HasPrintedWelcomeMessage = true;
        }
    }
}
```

And if everything works, it should look like this:

![The console output from the above program](unity-console.png)

And now, you're only limited by your creativity :)
