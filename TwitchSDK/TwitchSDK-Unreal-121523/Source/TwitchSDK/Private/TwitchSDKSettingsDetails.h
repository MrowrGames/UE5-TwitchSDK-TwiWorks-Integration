// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR
#include "TwitchSDK.h"
#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FTwitchSDKSettingsDetails : public IDetailCustomization
{
	FText StatusText;
	FText GetStatusText() const;

public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
#endif
