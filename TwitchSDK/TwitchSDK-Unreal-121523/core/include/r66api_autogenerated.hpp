#pragma once


namespace R66 {

    // For TwitchSDK --> TwiWorks
    struct AuthTokens {
        string AccessToken;
        string RefreshToken;
    };

    /**
     * @class IR66ApiCommon r66api_autogenerated.hpp r66.hpp
     * @brief The interface containing the API surface that is common across all environments.
     */
    class IR66ApiCommon {
    public:
        virtual ~IR66ApiCommon() {}
        /// <summary>
        /// Query the current authentication state.
        /// </summary>
        /// <remarks>
        /// This call always returns immediately.
        /// </remarks>
        /// <seealso cref="GetAuthenticationInfo" />
        virtual void GetAuthState(ResolveFn<const AuthState&> resolve, RejectFn reject) = 0;

        // For TwitchSDK --> TwiWorks
        virtual void GetAuthTokens(ResolveFn<const AuthTokens&> resolve, RejectFn reject) = 0;

        /// <summary>
        /// Start the authorization code flow. If a user is currently logged in, this does nothing.
        /// After a successfull call to GetAuthenticationInfo, subsequent calls will return the same AuthenticationInfo
        /// until the login is either completed or timed out.
        /// </summary>
        /// <remarks>
        /// Credentials are stored persistently, so after loggin in successfully and restarting your application,
        /// the library remains in the LoggedIn state.
        /// </remarks>
        /// <param name="scopes">The set of OAuth scopes you would like to request.</param>
        /// <seealso cref="GetAuthState" />
        /// <seealso cref="LogOut" />
        virtual void GetAuthenticationInfo(R66::string_view scopes, ResolveFn<const AuthenticationInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Log out.
        /// </summary>
        /// <remarks>
        /// This call always returns immediately.
        /// </remarks>
        virtual void LogOut(ResolveFn<void> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Gets information about the currently logged-in user's account.
        /// </summary>
        /// <remarks>
        /// If the user is not logged in, this also waits for a successful login.
        /// </remarks>
        virtual void GetMyUserInfo(ResolveFn<const UserInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Gets information about a specific Twitch user account.
        /// </summary>
        /// <remarks>
        /// Returns an empty object if no such user could be found.
        /// </remarks>
        /// <param name="id">The account ID to query for.</param>
        virtual void GetUserInfoById(R66::string_view id, ResolveFn<const UserInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Gets information about a specific Twitch user account.
        /// </summary>
        /// <remarks>
        /// Returns an empty object if no such user could be found.
        /// </remarks>
        /// <param name="login">The login name to query for.</param>
        virtual void GetUserInfoByLoginName(R66::string_view login, ResolveFn<const UserInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Gets information about the currently logged-in user's stream.
        /// </summary>
        /// <remarks>
        /// If the user is not logged in, this also waits for a successful login.
        /// Returns <c>null</c> if the user is currently not live.
        /// </remarks>
        virtual void GetMyStreamInfo(ResolveFn<const StreamInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Gets information about active streams. Streams are returned sorted by number of current viewers, in descending order.
        /// </summary>
        /// <remarks>
        /// Submitting an empty <paramref name="query" /> is valid and fetches the top streams currently on Twitch.
        /// Across multiple pages of results, there may be duplicate or missing streams, as viewers join and leave streams.
        /// </remarks>
        /// <param name="query">The query to submit to the API.</param>
        /// <returns>An object containing the matching streams and optionally a pagination token to fetch more results.</returns>
        virtual void QueryStreams(const StreamQuery& query, ResolveFn<const StreamQueryResult&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Checks if the currently logged-in user is subscribed to a specific channel.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>user:read:subscriptions</c></remarks>
        /// <param name="broadcaster">ID of the broadcaster to check for.</param>
        virtual void CheckUserSubscription(R66::string_view broadcaster, ResolveFn<const UserSubscriptionCheckResult&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Creates a clip programmatically. This returns both an ID and an edit URL for the new clip.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>clips:edit</c><para>Creating a clip can take up to 15 seconds.</para><para>
        /// If <paramref name="hasDelay" /> is <c>false</c>, the clip is captured from the live stream when the API is called; otherwise, a delay is added before the clip is captured
        /// (to account for the brief delay between the broadcaster’s stream and the viewer’s experience of that stream).
        /// </para></remarks>
        /// <param name="hasDelay">Whether stream delay should be added before capturing the clip.</param>
        /// <returns>An object describing the new clip.</returns>
        virtual void CreateClip(bool hasDelay, ResolveFn<const ClipInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Creates a marker in the stream of a user.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>channel:manage:broadcast</c><para>
        /// A marker is an arbitrary point in a stream that the broadcaster wants to mark;
        /// e.g., to easily return to later. The marker is created at the current timestamp in the
        /// live broadcast when the request is processed.
        /// Markers can be created by the stream owner or editors.
        /// </para><para>
        /// Markers cannot be created in some cases (an error will occur):
        /// <list type="bullet"><item><description>If the specified user’s stream is not live.</description></item><item><description>If VOD (past broadcast) storage is not enabled for the stream.</description></item><item><description>For premieres (live, first-viewing events that combine uploaded videos with live chat).</description></item><item><description>For reruns (subsequent (not live) streaming of any past broadcast, including past premieres).</description></item></list></para></remarks>
        /// <param name="description">Optional. Description of or comments on the marker. Max length is 140 characters.</param>
        /// <returns>An object describing the new stream marker.</returns>
        virtual void CreateStreamMarker(R66::string_view description, ResolveFn<const StreamMarkerInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Create a poll on the user's Twitch channel.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>channel:manage:polls</c><para>
        /// Creating a poll automatically subscribes to updates for this poll.
        /// Avoid unnecessary traffic by invoking <see cref="UnsubscribeFromPoll" /> at some point for every poll you create.
        /// </para></remarks>
        /// <returns>An object describing the new poll.</returns>
        virtual void CreatePoll(const PollDefinition& p, ResolveFn<const PollInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Wait for updates to a specified poll.
        /// Updates occur when users submit votes, and finally, when the poll ends.
        /// </summary>
        /// <remarks>
        /// Avoid calling this function on polls after they have ended, as they will no longer recieve updates at that point.
        /// </remarks>
        /// <param name="poll">Id of the poll to wait for.</param>
        /// <returns>The updated poll info.</returns>
        virtual void WaitForPollUpdate(R66::string_view poll, ResolveFn<const PollInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// End a poll prematurely.
        /// </summary>
        /// <returns>The updated poll info.</returns>
        virtual void EndPoll(const EndPollRequest& req, ResolveFn<const PollInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Unsubscribe from updates for a specific poll.
        /// </summary>
        /// <remarks>
        /// You must not invoke this function multiple times for the same poll.
        /// Unsubscribing from polls is optional. Failing to do so results in unnecessary traffic,
        /// but will not cause any malfunction or memory leak.
        /// </remarks>
        /// <param name="id">Id of the poll to unsubscribe from.</param>
        virtual void UnsubscribeFromPoll(R66::string_view id, ResolveFn<void> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Create a Channel Points Prediction on the user's Twitch channel.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>channel:manage:predictions</c><para>
        /// Creating a prediction automatically subscribes to updates for this prediction.
        /// Avoid unnecessary traffic by invoking <see cref="UnsubscribeFromPrediction" /> at some point for every poll you create.
        /// </para></remarks>
        /// <returns>An object describing the new prediction.</returns>
        virtual void CreatePrediction(const PredictionDefinition& p, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Wait for updates to a specified prediction.
        /// Updates occur when users submit votes, and finally, when the prediction ends.
        /// </summary>
        /// <remarks>
        /// Avoid calling this function on predictions after they have ended, as they will no longer recieve updates at that point.
        /// </remarks>
        /// <param name="prediction">Id of the prediction to wait for.</param>
        /// <returns>The updated prediction info.</returns>
        virtual void WaitForPredictionUpdate(R66::string_view prediction, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Lock, resolve, or cancel a Channel Points Prediction.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>channel:manage:predictions</c></remarks>
        /// <returns>The updated prediction info.</returns>
        virtual void EndPrediction(const EndPredictionRequest& req, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Unsubscribe from updates for a specific prediction.
        /// </summary>
        /// <remarks>
        /// You must not invoke this function multiple times for the same prediction.
        /// Unsubscribing from predictions is optional. Failing to do so results in unnecessary traffic,
        /// but will not cause any malfunction or memory leak.
        /// </remarks>
        /// <param name="id">Id of the prediction to unsubscribe from.</param>
        virtual void UnsubscribeFromPrediction(R66::string_view id, ResolveFn<void> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Modifies channel information for the currently logged-in user.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>channel:manage:broadcast</c></remarks>
        virtual void ModifyChannelInformation(const ModifyChannelInfoRequest& req, ResolveFn<void> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Gets a ranked list of Bits leaderboard information for an authorized broadcaster.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>bits:read</c></remarks>
        virtual void GetBitsLeaderboard(const BitsLeaderboardRequest& req, ResolveFn<const BitsLeaderboard&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Updates the set of available custom channel points rewards on a channel.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>channel:manage:redemptions</c><para>
        /// If you enable any custom rewards, please clear them by invoking this function with an empty
        /// rewards list when the game terminates.
        /// </para><para>
        /// Note that removing rewards that still have pending redemptions will set those redemptions
        /// to <see cref="CustomRewardRedemptionState.Fulfilled" />.
        /// </para></remarks>
        virtual void ReplaceCustomRewards(const CustomRewardList& req, ResolveFn<void> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Updates the status of Custom Reward Redemption objects on a channel that are in the <see cref="CustomRewardRedemptionState.Unfulfilled" /> status.
        /// </summary>
        /// <remarks>
        /// Required scope: <c>channel:manage:redemptions</c></remarks>
        /// <seealso cref="SubscribeToEventStream" />
        /// <seealso cref="WaitForCustomRewardEvent" />
        virtual void ResolveCustomReward(const CustomRewardResolveRequest& req, ResolveFn<void> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Subscribe to a Twitch EventSub stream.
        /// </summary>
        /// <remarks>
        /// You must periodically receive events from your event stream.
        /// You must call <see cref="CloseEventStream" /> once you are done.
        /// <para>
        /// To avoid leaking memory indefinitely in this case, there is an internal limit of 512 buffered
        /// events per event stream subscription, as well as an internal limit of 64 subscription per event type in total.
        /// When the internal buffer of a subscription is full, incoming events will be discarded silently.
        /// When the subscriptions limit is reached, an arbitrary subscription of the same type will be closed, and
        /// a warning message will be logged.
        /// </para></remarks>
        /// <returns>An object describing the subscription.</returns>
        /// <seealso cref="WaitForChannelSubscribeEvent" />
        /// <seealso cref="WaitForChannelFollowEvent" />
        /// <seealso cref="WaitForChannelCheerEvent" />
        /// <seealso cref="WaitForCustomRewardEvent" />
        /// <seealso cref="WaitForHypeTrainEvent" />
        /// <seealso cref="WaitForChannelRaidEvent" />
        virtual void SubscribeToEventStream(const EventStreamRequest& req, ResolveFn<const EventStreamDesc&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// A notification when a specified channel receives a subscriber. This does not include resubscribes.
        /// </summary>
        /// <remarks>You may only call this with a subscription for the correct event type.</remarks>
        /// <param name="desc">An object describing the subscription.</param>
        /// <returns>The event.</returns>
        /// <seealso cref="SubscribeToEventStream" />
        virtual void WaitForChannelSubscribeEvent(const EventStreamDesc& desc, ResolveFn<const ChannelSubscribeEvent&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// A specified channel receives a follow.
        /// </summary>
        /// <remarks>You may only call this with a subscription for the correct event type.</remarks>
        /// <param name="desc">An object describing the subscription.</param>
        /// <returns>The event.</returns>
        /// <seealso cref="SubscribeToEventStream" />
        virtual void WaitForChannelFollowEvent(const EventStreamDesc& desc, ResolveFn<const ChannelFollowEvent&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// A user cheers on the specified channel.
        /// </summary>
        /// <remarks>You may only call this with a subscription for the correct event type.</remarks>
        /// <param name="desc">An object describing the subscription.</param>
        /// <returns>The event.</returns>
        /// <seealso cref="SubscribeToEventStream" />
        virtual void WaitForChannelCheerEvent(const EventStreamDesc& desc, ResolveFn<const ChannelCheerEvent&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// A viewer has redeemed a custom channel points reward on the specified channel or the redemption
        /// has been updated (i.e., fulfilled or cancelled).
        /// </summary>
        /// <remarks>You may only call this with a subscription for the correct event type.</remarks>
        /// <param name="desc">An object describing the subscription.</param>
        /// <returns>The event.</returns>
        /// <seealso cref="SubscribeToEventStream" />
        /// <seealso cref="ReplaceCustomRewards" />
        virtual void WaitForCustomRewardEvent(const EventStreamDesc& desc, ResolveFn<const CustomRewardEvent&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// A Hype Train makes progress on the user's channel.
        /// Requires the <c>channel:read:hype_train</c> scope.
        /// </summary>
        /// <remarks>You may only call this with a subscription for the correct event type.</remarks>
        /// <param name="desc">An object describing the subscription.</param>
        /// <returns>The event.</returns>
        /// <seealso cref="SubscribeToEventStream" />
        virtual void WaitForHypeTrainEvent(const EventStreamDesc& desc, ResolveFn<const HypeTrainEvent&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// A broadcaster raids another broadcaster’s channel.
        /// </summary>
        /// <remarks>You may only call this with a subscription for the correct event type.</remarks>
        /// <param name="desc">An object describing the subscription.</param>
        /// <returns>The event.</returns>
        /// <seealso cref="SubscribeToEventStream" />
        virtual void WaitForChannelRaidEvent(const EventStreamDesc& desc, ResolveFn<const ChannelRaidEvent&> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Close a Twitch EventSub subscription.
        /// </summary>
        /// <remarks>
        /// After the subscription is closed, trying to receive events from it is an error.
        /// </remarks>
        /// <param name="desc">An object describing the subscription.</param>
        virtual void CloseEventStream(const EventStreamDesc& desc, ResolveFn<void> resolve, RejectFn reject) = 0;
        /// <summary>
        /// Prepare for shutdown.
        /// </summary>
        /// <remarks>
        /// You should invoke this function and wait for it to return before exiting from your game process.
        /// This function resets state associated with your session such as available channel points rewards.
        /// </remarks>
        virtual void PrepareShutdown(ResolveFn<void> resolve, RejectFn reject) = 0;
    };
}
