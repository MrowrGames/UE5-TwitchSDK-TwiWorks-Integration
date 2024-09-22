#include "ApiGetUserTokens.h"

// For TwitchSDK --> TwiWorks

void UApiGetUserTokens::Activate()
{
	TWeakObjectPtr<UApiGetUserTokens> weak(this);
	auto exception_handler = [weak](const std::exception& e)
	{
		if (weak.IsValid() && weak->Error.IsBound())
		{
			weak->Error.Broadcast(FString(), FString(e.what()));
		}
		else
			UE_LOG(LogTwitchSDK, Error, TEXT("GetUserTokens error: %s"), UTF8_TO_TCHAR(e.what()));
	};
	try
	{
		FTwitchSDKModule::Get().Core->GetAuthTokens(
			[weak](const R66::AuthTokens& r)
			{
				if (!weak.IsValid())
				{
					return;
				}
				weak->Done.Broadcast(FString(r.AccessToken.c_str()), FString(r.RefreshToken.c_str()));
			},
			exception_handler
		);
	}
	catch (const std::exception& e)
	{
		exception_handler(e);
	}
}

UApiGetUserTokens* UApiGetUserTokens::GetUserTokens()
{
	auto ptr = NewObject<UApiGetUserTokens>();
	return ptr;
}
