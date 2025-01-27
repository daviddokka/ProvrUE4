// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Managers/ProVRViewManager.h"
#include "Views/ProVRWidgetBase.h"
#include "ProVRPawn.h"
#include "Kismet/GameplayStatics.h"
#include "ProVRGameInstance.h"
#include "Engine/World.h"
#include "UObject/UObjectGlobals.h"

UProVRViewManager::UProVRViewManager()
{

	
	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) return;
	
	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> WelcomeViewAsset(TEXT("/Game/Widgets/LoginRegister/UI_ProVR_WelcomeView"));
	if (WelcomeViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Welcome, NewObject<UProVRWidgetBase>(this, WelcomeViewAsset.Class, TEXT("UI_ProVR_WelcomeView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> RegisterViewAsset(TEXT("/Game/Widgets/LoginRegister/UI_ProVR_RegisterView"));
	if (RegisterViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Register, NewObject<UProVRWidgetBase>(this, RegisterViewAsset.Class, TEXT("UI_ProVR_RegisterView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> LoginViewAsset(TEXT("/Game/Widgets/LoginRegister/UI_ProVR_LoginView"));
	if (LoginViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::LoginView, NewObject<UProVRWidgetBase>(this, LoginViewAsset.Class, TEXT("UI_ProVR_LoginView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> LogoutViewAsset(TEXT("/Game/Widgets/LoginRegister/UI_ProVR_LogoutView"));
	if (LogoutViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Logout, NewObject<UProVRWidgetBase>(this, LogoutViewAsset.Class, TEXT("UI_ProVR_LogoutView")));
	}


	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> ModelViewAsset(TEXT("/Game/Widgets/UI_ProVR_ModelView"));
	if (ModelViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::ModelView, NewObject<UProVRWidgetBase>(this, ModelViewAsset.Class, TEXT("UI_ProVR_ModelView")));
	}


	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> PauseMenuAsset(TEXT("/Game/Widgets/UI_ProVR_PauseMenu"));
	if (PauseMenuAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::PauseMenu, NewObject<UProVRWidgetBase>(this, PauseMenuAsset.Class, TEXT("UI_ProVR_PauseMenu")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> MainMenuAsset(TEXT("/Game/Widgets/UI_ProVR_MainMenu"));
	if (MainMenuAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::MainMenu, NewObject<UProVRWidgetBase>(this, MainMenuAsset.Class, TEXT("UI_ProVR_MainMenu")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> SessionViewAsset(TEXT("/Game/Widgets/Sessions/UI_ProVR_SessionView"));
	if (SessionViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::SessionView, NewObject<UProVRWidgetBase>(this, SessionViewAsset.Class, TEXT("UI_ProVR_SessionView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> CreateSessionViewAsset(TEXT("/Game/Widgets/Sessions/UI_ProVR_CreateSessionView"));
	if (CreateSessionViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::CreateSessionView, NewObject<UProVRWidgetBase>(this, CreateSessionViewAsset.Class, TEXT("UI_ProVR_CreateSessionView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> OptionsMenuAsset(TEXT("/Game/Widgets/UI_ProVR_OptionsMenu"));
	if (OptionsMenuAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::OptionsMenu, NewObject<UProVRWidgetBase>(this, OptionsMenuAsset.Class, TEXT("UI_ProVR_OptionsMenu")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> ParticipantHUDAsset(TEXT("/Game/Widgets/UI_ProVR_ParticipantHUD"));
	if (ParticipantHUDAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::ParticipantHUD, NewObject<UProVRWidgetBase>(this, ParticipantHUDAsset.Class, TEXT("UI_ProVR_ParticipantHUD")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> BrowseSessionViewAsset(TEXT("/Game/Widgets/Sessions/UI_ProVR_BrowseSessionView"));
	if (BrowseSessionViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::BrowseSessionView, NewObject<UProVRWidgetBase>(this, BrowseSessionViewAsset.Class, TEXT("UI_ProVR_BrowseSessionView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> NoMenuViewAsset(TEXT("/Game/Widgets/UI_ProVR_NoMenu"));
	if (NoMenuViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::NoMenu, NewObject<UProVRWidgetBase>(this, NoMenuViewAsset.Class, TEXT("UI_ProVR_NoMenu")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> InviteViewAsset(TEXT("/Game/Widgets/Sessions/UI_ProVR_InviteView"));
	if (InviteViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::InviteView, NewObject<UProVRWidgetBase>(this, InviteViewAsset.Class, TEXT("UI_ProVR_InviteView")));
	}
	
	/*
	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> RegisterViewAsset(TEXT("/Game/Widgets/UI_ProVR_RegisterView"));
	if (RegisterViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Register, RegisterViewAsset);
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> LoginViewAsset(TEXT("/Game/Widgets/UI_ProVR_LoginView"));
	if (LoginViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Login,  LoginViewAsset);
	}
	*/
}

void UProVRViewManager::Tick(float DeltaTime)
{
	UProVRManagerBase::Tick(DeltaTime);


}

void UProVRViewManager::SwitchView(EProVRView NextView)
{

	if (CurrentView != NextView)
	{
		if (ViewWidgetMap.Contains(CurrentView))
		{
			ViewWidgetMap[CurrentView]->RemoveFromParent();
		}

		if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
		{
			if (UWorld* World = GameInstance->GetWorld())
			{
				if (APlayerController* PlayerController = World->GetFirstPlayerController())
				{
					if (APawn* Pawn = PlayerController->GetPawn())
					{
						AProVRPawn* Pawn_ = Cast<AProVRPawn>(Pawn);
						Pawn_->WidgetComp->SetWidget(ViewWidgetMap[NextView]);
					}
				}
			}
		}
	}
}