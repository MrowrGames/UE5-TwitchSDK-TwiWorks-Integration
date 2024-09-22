# EventSub

Twitch provides the [EventSub](https://dev.twitch.tv/docs/eventsub) API to allow developers to subscribe to events (such as Channel Follow, Channel Subscribe, etc) and receive notifications.

The plugin currently supports the following [Subscription Types](@ref R66::EventStreamKind):

- Subscription
- Follower
- Cheer
- Custom Rewards (i.e., Channel Points)
- Hype Train
- Channel Raid

Subscribing to these events always involves a three-step process:

## Unreal Engine (C++)

### Create a Subscription

First, subscribe to the event stream using the respective method:

```
auto stream = core->SubscribeToChannelFollowEvents();
```

### Wait for Events

```
stream.WaitForEvent([](const R66::ChannelFollowEvent& e) {
	auto userName = R66::ToFString(e.UserDisplayName);
	auto message = FString::Printf(TEXT("%s just followed!"), *userName);
	ShowMessageToPlayer(message);
});
```

### Close the Subscription

The @ref R66::TwitchEventStream class's destructor automatically unsubscribes from the event stream for you.
You can simply destroy the object.

## Unreal Engine (Blueprints)

### Create a Subscription

![](ue4-bp-es-1.png)

### Wait for Events

![](ue4-bp-es-2.png)

### Close the Subscription

![](ue4-bp-es-3.png)

You should close the event stream once you no longer need it.

