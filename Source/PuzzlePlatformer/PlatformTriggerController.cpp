// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTriggerController.h"

#include "Components/BoxComponent.h"
#include "MovingPlatformController.h"

// Sets default values
APlatformTriggerController::APlatformTriggerController() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr)) return;

	RootComponent = TriggerVolume;

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTriggerController::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTriggerController::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APlatformTriggerController::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void APlatformTriggerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void APlatformTriggerController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Activated"));
	UE_LOG(LogTemp, Warning, TEXT("[%s] - [%s]"), *(OtherActor->GetName()), *(OtherComp->GetName()));

	for (AMovingPlatformController* Platform : PlatformsToTrigger) {
		Platform->AddActiveTrigger();
	}
}

void APlatformTriggerController::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("Deactivated"));

	for (AMovingPlatformController* Platform : PlatformsToTrigger) {
		Platform->RemoveActiveTrigger();
	}
}

