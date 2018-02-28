// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UPuzzlePlatformerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPuzzlePlatformerGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init();

	UFUNCTION(Exec)
    void Host();
	
	UFUNCTION(Exec)
	void Join(const FString& Address);
	
	
	
};
