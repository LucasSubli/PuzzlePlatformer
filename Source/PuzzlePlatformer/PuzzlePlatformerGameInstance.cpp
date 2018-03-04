// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformerGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "GameFramework/GameModeBase.h"


const static FName SESSION_NAME = TEXT("My Session Game");


UPuzzlePlatformerGameInstance::UPuzzlePlatformerGameInstance(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformerGameInstance::Init() {
	Super::Init();

	IOnlineSubsystem* OnlineSubs = IOnlineSubsystem::Get();
	if (OnlineSubs != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *OnlineSubs->GetSubsystemName().ToString());
		SessionInterface = OnlineSubs->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformerGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformerGameInstance::OnDestroySessionComplete);
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Found no online subsystem"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());

}

void UPuzzlePlatformerGameInstance::LoadMenuWidget() {

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	if (UMainMenu* Menu = CreateWidget<UMainMenu>(PlayerController, MenuClass)) {
		Menu->AddToPlayerScreen();
		Menu->SetMenuInterface(this);
	}
}

void UPuzzlePlatformerGameInstance::LoadInGameMenu() { 
	if (!ensure(InGameMenuClass != nullptr)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	
	if (UMenuWidget* Menu = CreateWidget<UMenuWidget>(PlayerController, InGameMenuClass)) {
		Menu->AddToPlayerScreen();
		Menu->SetMenuInterface(this);
	}
}


void UPuzzlePlatformerGameInstance::Host() {
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(SESSION_NAME);
		} else {
			CreateSession();
		}
	}
}

void UPuzzlePlatformerGameInstance::Join(const FString& Address) {
	UEngine * Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformerGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (World->IsServer()) {
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode) {
			GameMode->ReturnToMainMenuHost();
		}
	} else {
		PlayerController->ClientReturnToMainMenu("Back to main menu");
	}
}

void UPuzzlePlatformerGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

void UPuzzlePlatformerGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformerGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (Menu != nullptr) {
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}