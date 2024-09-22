// Copyright Epic Games, Inc. All Rights Reserved.

#include "TwitchSDKBPLibrary.h"
#include "TwitchSDK.h"
#include "Misc/Char.h"

UTwitchSDKBPLibrary::UTwitchSDKBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FString UTwitchSDKBPLibrary::BuildOAuthScopes(TArray<FTwitchSDKOAuthScope> Scopes, TArray<FString> CustomScopes) {
	FString result;

	for (auto s : Scopes)
	{
		FString enumName = StaticEnum<FTwitchSDKOAuthScope>()->GetNameStringByValue((int64)s);
		// do some magic to turn this into a scope name:
		bool noColon = true;
		for (auto c : enumName)
		{
			if (FChar::IsUpper(c) && !noColon)
				result += TEXT(':');
			result += FChar::ToLower(c);
			noColon = c == '_'; // after the first iteration, generate colons unless they're following an underscore
		}

		result += TEXT(" ");
	}

	for (auto& s : CustomScopes)
	{
		result += s;

		result += TEXT(" ");
	}

	result.TrimEndInline();
	return result;
}
