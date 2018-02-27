// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatformController.h"


AMovingPlatformController::AMovingPlatformController() {
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatformController::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();

	GlobalTargetLocation =  GetTransform().TransformPosition(TargetLocation);  // TargetLocation is Local Space
}

void AMovingPlatformController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		if (ActiveTriggers > 0) {
			FVector Location = GetActorLocation();
			float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			float JourneyTravelled = (Location - GlobalStartLocation).Size();

			if (JourneyTravelled > JourneyLength) { // If the target locations is passed
				Swap(GlobalTargetLocation, GlobalStartLocation);
			}

			FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			Location += Speed * DeltaTime * Direction;
			SetActorLocation(Location);
		}
	}

}

void AMovingPlatformController::AddActiveTrigger() {
	ActiveTriggers++;
}

void AMovingPlatformController::RemoveActiveTrigger() {
	ActiveTriggers--;
}