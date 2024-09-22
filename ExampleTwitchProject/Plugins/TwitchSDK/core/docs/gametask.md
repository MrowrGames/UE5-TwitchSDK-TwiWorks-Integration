# GameTask

### Why GameTask?

When designing the Unity plugin, we had one problem:

For a lot of actions, we need to talk to the Twitch API. But talking to the Twitch API takes time, and freezing the game is not an option.

For example, when you create a poll, we need to wait for the Twitch server to confirm that it was successfully created - we can't just blindly assume that it will work out.

> What if the user isn't allowed to create polls? What if the Twitch servers are down? What if the user's internet is down? What if another poll is already running? What if you've set the poll up incorrectly (e.g. duration less than 15 seconds?)

But we also can't just freeze the game while we wait for a result. That would be a bit easier for the game developer, but horrible for the player.

So, most APIs return a `GameTask`. A `GameTask` has three important properties:

* **IsCompleted**: this tells you if the action has finished and a result is available. It means that either `MaybeResult` or `Exception` is not `null`.
* **Exception**: if the action has failed for some reason, this will tell you more about what went wrong. Returns `null` if the action has succeeded or hasn't completed yet.
* **MaybeResult**: this will be `null` until the action was completed. On success, this returns the action's return value. On error, accessing this property throws an exception containing information about what went wrong.


So, if you (for example) create a poll, you need to wait a couple of frames until `MaybeResult` returns a `Poll` object that lets you get information like the current vote status, and finally the result of the poll.

Or, if you want to know how many people are watching, that information may not be readily available. Imagine the user just logged in, it may take a second for us to get the viewcount. 

So if you call `GetMyStreamInfo()`, you get a `GameTask<StreamInfo>` that'll tell you all the info like the view count, etc. - once it has the information ready.

That can be immediately, but it might be a couple of frames in the future. So, you can save the value and look at it in a future `Update` of your component.

For read-only methods like `GetMyStreamInfo()`, you can also just call them every `Update()`, it won't send a request every time. However, you should not do this for "write" operations, like `NewPoll`.
