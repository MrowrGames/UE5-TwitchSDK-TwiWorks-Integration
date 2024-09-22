#include "ApiGetAuthTokens.h"

// For TwitchSDK --> TwiWorks

void UApiGetAuthTokens::Activate()
{
	TWeakObjectPtr<UApiGetAuthTokens> weak(this);
	auto exception_handler = [weak](const std::exception& e)
	{
		if (weak.IsValid() && weak->Error.IsBound())
		{
			weak->Error.Broadcast(FString(), FString(e.what()));
		}
		else
			UE_LOG(LogTwitchSDK, Error, TEXT("GetAuthTokens error: %s"), UTF8_TO_TCHAR(e.what()));
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

UApiGetAuthTokens* UApiGetAuthTokens::GetAuthTokens()
{
	auto ptr = NewObject<UApiGetAuthTokens>();
	return ptr;
}
