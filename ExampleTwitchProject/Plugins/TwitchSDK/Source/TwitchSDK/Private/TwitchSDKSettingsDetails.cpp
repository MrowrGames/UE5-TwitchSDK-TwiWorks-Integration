#if WITH_EDITOR

#include "TwitchSDKSettingsDetails.h"
#include "TwitchSDKSettings.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Json/Public/Json.h"


#define LOCTEXT_NAMESPACE "TwitchSDKSettingsDetails"

TSharedRef<IDetailCustomization> FTwitchSDKSettingsDetails::MakeInstance() {
	return MakeShareable(new FTwitchSDKSettingsDetails);
}

void FTwitchSDKSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);
	check(ObjectsBeingCustomized.Num() == 1);
	TWeakObjectPtr<UTwitchSDKSettings> Settings = Cast<UTwitchSDKSettings>(ObjectsBeingCustomized[0].Get());

	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Authentication");

	Category.AddCustomRow(LOCTEXT("Version", "Version"))
		.NameContent()
		[
			SNew(STextBlock).Text(LOCTEXT("Version", "Version"))
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(5)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(FText::FromString(R66::ToFString(R66::Version)))
			]
		];
	Category.AddCustomRow(LOCTEXT("CredentialsTester", "CredentialsTester"))
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(5)
			.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("TestR66Credentials", "Test Credentials"))
				.OnClicked_Lambda([this, Settings]()
				{
					FHttpRequestRef req = FHttpModule::Get().CreateRequest();

					// try to run the client_credentials flow to verify the validity of both client-id and secret
					FString url = FString::Format(TEXT("https://id.twitch.tv/oauth2/token?client_id={0}&client_secret=dummy&grant_type=client_credentials"), { *Settings->ClientId });
					req->SetURL(url);
					req->SetVerb(TEXT("POST"));
					req->SetContentAsString(TEXT(""));

					FString userAgent = FString::Printf(TEXT("%s Twitch-Route-66/0.1"), *FGenericPlatformHttp::GetDefaultUserAgent());
					req->SetHeader(TEXT("User-Agent"), userAgent);

					req->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr req, FHttpResponsePtr resp, bool success) {
						if (!success)
						{
							this->StatusText = LOCTEXT("R66StatusNoConnection", "No internet connection.");
						}

						auto responseCode = resp->GetResponseCode();
						if (responseCode == 200)
						{
							this->StatusText = LOCTEXT("R66StatusOk", "Success!");
						}
						else
						{
							TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(resp->GetContentAsString());
							TSharedPtr<FJsonValue> json;
							FJsonSerializer::Deserialize(reader, json);
							const TSharedPtr<FJsonObject>* jsonRoot;
							FString errorMessage;
							FString newStatus;
							if (json.IsValid() && json->TryGetObject(jsonRoot) && (*jsonRoot)->TryGetStringField(TEXT("message"), errorMessage))
							{
								newStatus = FString::Format(TEXT("Error: {0}"), { *errorMessage });
							}
							else
							{
								newStatus = FString::Format(TEXT("HTTP Error {0}: {1}"), { resp->GetResponseCode(), *resp->GetContentAsString() });
							}
							this->StatusText = FText::AsCultureInvariant(newStatus);
						}
					});
					req->ProcessRequest();

					this->StatusText = LOCTEXT("R66StatusTesting", "Testing ...");
					return(FReply::Handled());
				})
			]
			+ SHorizontalBox::Slot()
			.Padding(5)
			.VAlign(EVerticalAlignment::VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(this, &FTwitchSDKSettingsDetails::GetStatusText)
			]
		];
}

FText FTwitchSDKSettingsDetails::GetStatusText() const {
	return StatusText;
}

#undef LOCTEXT_NAMESPACE

#endif
