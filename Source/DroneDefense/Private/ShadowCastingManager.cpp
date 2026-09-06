// Fill out your copyright notice in the Description page of Project Settings.

#include "ShadowCastingManager.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerController.h"
#include "DroneDefense/DroneDefenseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <limits>

// Sets default values
AShadowCastingManager::AShadowCastingManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UpdateTimer = 0.0f;
}

// Called when the game starts or when spawned
void AShadowCastingManager::BeginPlay()
{
	Super::BeginPlay();
	CollectShadowTargets();
	UpdateShadowCasting();
}

// Called every frame
void AShadowCastingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShadowCastingManager::UpdateShadowCasting()
{
	if (ShadowEnableDistance <= 0.0f)
	{
		return;
	}

	APlayerController* PlayerController =
		UGameplayStatics::GetPlayerController(this, 0);

	if (!PlayerController)
	{
		return;
	}

	APlayerCameraManager* CameraManager =
		PlayerController->PlayerCameraManager;

	if (!CameraManager)
	{
		return;
	}

	//TArray<TObjectPtr<FShadowCastingData>> ShadowTargets;
	for (auto& Target : ShadowTargets)
	{
		UStaticMeshComponent* MeshComponent =
			Target.MeshComponent;

		if (!IsValid(MeshComponent))
		{
			continue;
		}

		const float DistanceSquared =
			GetDistanceSquaredToCamera(MeshComponent);

		const float EnableDistanceSquared =
			FMath::Square(ShadowEnableDistance);

		const float DisableDistanceSquared =
			FMath::Square(ShadowDisableDistance);

		bool bShouldCastShadow = Target.bIsCastingShadow;

		// 현재 그림자가 OFF인 경우
		if (!Target.bIsCastingShadow)
		{
			// 충분히 가까워졌을 때만 ON
			if (DistanceSquared <= EnableDistanceSquared)
			{
				bShouldCastShadow = true;
			}
		}
		// 현재 그림자가 ON인 경우
		else
		{
			// 충분히 멀어졌을 때만 OFF
			if (DistanceSquared >= DisableDistanceSquared)
			{
				bShouldCastShadow = false;
			}
		}

		if (bShouldCastShadow != Target.bIsCastingShadow)
		{
			MeshComponent->SetCastShadow(bShouldCastShadow);

			Target.bIsCastingShadow = bShouldCastShadow;
		}
	}
}

float AShadowCastingManager::GetDistanceSquaredToCamera(UStaticMeshComponent* MeshComponent)
{
	APlayerController* PlayerController =
		UGameplayStatics::GetPlayerController(this, 0);

	APlayerCameraManager* CameraManager =
		PlayerController->PlayerCameraManager;

	FVector CameraLoc = CameraManager->GetCameraLocation();

	FVector DistVec = CameraLoc - MeshComponent->GetComponentLocation();
	return FMath::Square(DistVec.Size());

}

void AShadowCastingManager::CollectShadowTargets()
{
	ShadowTargets.Empty();

	TArray<AActor*> StaticMeshActors;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AStaticMeshActor::StaticClass(),
		StaticMeshActors
	);

	for (AActor* Actor : StaticMeshActors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}

		UStaticMeshComponent* MeshComponent =
			Actor->FindComponentByClass<UStaticMeshComponent>();

		if (!IsValid(MeshComponent))
		{
			continue;
		}

		if (!MeshComponent->IsRegistered())
		{
			continue;
		}

		FShadowCastingData Data;

		Data.MeshComponent = MeshComponent;
		Data.bIsCastingShadow = MeshComponent->CastShadow;

		ShadowTargets.Add(Data);
	}
}

