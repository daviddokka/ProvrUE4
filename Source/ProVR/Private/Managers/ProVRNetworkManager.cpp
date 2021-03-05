// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Managers/ProVRNetworkManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

void UProVRNetworkManager::PushNewHttpRequest(UProVRHttpRequest* NewHttpRequest)
{
	ActiveHttpRequests.AddUnique(NewHttpRequest);
}

void UProVRNetworkManager::RemoveHttpRequest(UProVRHttpRequest* HttpRequest)
{
	ActiveHttpRequests.Remove(HttpRequest);
}

void UProVRNetworkManager::PerformLoginRequest(const FString& EmailAddress, const FString& Password, TFunction<void(bool)> OnCompleted)
{
	LastEmailAddress = EmailAddress;
	LastPassword = Password;

	if (OngoingTryRenewingAuthTokenRequest.IsValid())
	{
		bWasOngoingTryRenewingAuthTokenRequestCanceled = true;
		bTryRenewingAuthTokenRequestIsOngoing = false;
		OngoingTryRenewingAuthTokenRequest->CancelRequest();
	}

	TryRenewingAuthToken(OnCompleted);
}

void UProVRNetworkManager::TryRenewingAuthToken(TFunction<void(bool)> OnCompleted)
{
	OngoingTryRenewingAuthTokenRequestSubscribers.Add(OnCompleted);

	if (bTryRenewingAuthTokenRequestIsOngoing) return;
	bTryRenewingAuthTokenRequestIsOngoing = true;

	OngoingTryRenewingAuthTokenRequest = FHttpModule::Get().CreateRequest();

	OngoingTryRenewingAuthTokenRequest->OnProcessRequestComplete().BindUObject(this, &UProVRNetworkManager::OnRenewingAuthTokenRequestCompleted);

	OngoingTryRenewingAuthTokenRequest->SetURL(BACKEND_BASE_URL + AUTH_SERVICE_LOGIN_REQUEST_PATH);
	OngoingTryRenewingAuthTokenRequest->SetVerb("POST");

	TSharedPtr<FJsonObject> LoginRequestContent = MakeShareable(new FJsonObject);
	LoginRequestContent->SetStringField("emailAddress", LastEmailAddress);
	LoginRequestContent->SetStringField("password", LastPassword);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(LoginRequestContent.ToSharedRef(), Writer);
	OngoingTryRenewingAuthTokenRequest->SetContentAsString(OutputString);

	if (!OngoingTryRenewingAuthTokenRequest->ProcessRequest())
	{
		CallSubscribersAfterTryRenewingAuthTokenResponse(false);
	}
}

void UProVRNetworkManager::OnRenewingAuthTokenRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasOngoingTryRenewingAuthTokenRequestCanceled)
	{
		//If this callback is called due to the cancellation (meaning that there is another request upcoming, do not call subscribers.
		bWasOngoingTryRenewingAuthTokenRequestCanceled = false;
		return;
	}

	bTryRenewingAuthTokenRequestIsOngoing = false;

	if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		CallSubscribersAfterTryRenewingAuthTokenResponse(false);
	}
	else
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		if (!FJsonSerializer::Deserialize(JsonReader, JsonObject)
			|| !JsonObject.IsValid()
			|| !JsonObject->HasTypedField<EJson::String>("token"))
		{
			CallSubscribersAfterTryRenewingAuthTokenResponse(false);
		}
		else
		{
			CurrentAuthToken = JsonObject->GetStringField("token");

			CallSubscribersAfterTryRenewingAuthTokenResponse(true);
		}
	}

	OngoingTryRenewingAuthTokenRequest.Reset();
}

void UProVRNetworkManager::CallSubscribersAfterTryRenewingAuthTokenResponse(bool bSuccess)
{
	for (auto& Subscriber : OngoingTryRenewingAuthTokenRequestSubscribers)
	{
		if (Subscriber)
		{
			Subscriber(bSuccess);
		}
	}

	OngoingTryRenewingAuthTokenRequestSubscribers.Empty(); //Clear the array
}