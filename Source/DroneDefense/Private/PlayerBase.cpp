// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "DroneContainer.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerBase::GenerateDroneContainer(UStaticMeshComponent* FollowPosition)
{
	if (!DroneContainerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DroneContainerClass is not set"));
		return;
	}

	_droneContainer = GetWorld()->SpawnActor<ADroneContainer>(DroneContainerClass);
	_droneContainer->OperateDroneContainer(FollowPosition);
}

void APlayerBase::GenerateDroneInput()
{
	_droneContainer->GenerateDrone();
}

void APlayerBase::ChangeDroneModeInput()
{
	_droneContainer->ChangeDroneMode();
}