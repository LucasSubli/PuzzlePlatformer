// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"



bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinButton != nullptr)) return false;
	CancelJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmJoinButton != nullptr)) return false;
	ConfirmJoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
	}

void UMainMenu::HostServer() {
	if (MenuInterface != nullptr) {
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer() {
	if (MenuInterface != nullptr) {
		if (!ensure(IpTextBox != nullptr)) return;
		const FString& Address = IpTextBox->GetText().ToString();
		MenuInterface->Join(Address);
	}
}

void UMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame() {
	UWorld * World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController * PlayerController = GetOwningPlayer();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ConsoleCommand("quit");

	// Alternative
	//UWorld* World = GetWorld();
	//if (!ensure(World))return;
	//APlayerController* PlayerController = World->GetFirstPlayerController();
	//if (!ensure(PlayerController))return;
	//UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit);
}
