// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "DroneContainer.generated.h"

UENUM(BlueprintType)
enum class E_DroneState_Type : uint8
{
	Standard = 0 UMETA(DisplayName = "Standard"),
	Tracing UMETA(DisplayName = "Tracing"),
	Horizontal UMETA(DisplayName = "Horizontal"),

	SIZE UMETA(DisplayName = "Horizontal")
};

UCLASS()
class ADroneContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroneContainer();

protected:
#pragma region Follow Position
	UPROPERTY()
	class UStaticMeshComponent* _followPosition = nullptr;
#pragma endregion

#pragma region Drone Core
	UPROPERTY()
	UStaticMeshComponent* _droneCore = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Drone Core")
	float _coreRotationSpeed = 10.0f;
#pragma endregion

#pragma region Orbital
	UPROPERTY()
	UStaticMeshComponent* _orbitalCenter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "1.0"), Category = "Drone|Orbital")
	float _followCenterRate = 0.5f;

	float _standardAngle = 0.0f;

	UPROPERTY()
	TArray<UStaticMeshComponent*> _droneOrbitalArray;

	UPROPERTY()
	UStaticMeshComponent* _orbitalStandard = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Orbital")
	float _rotationSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Orbital")
	float _rotationRadius = 10.0f;
#pragma endregion

#pragma region Drone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Drone")
	TSubclassOf<class ADroneBase> _droneClass = nullptr;

	UPROPERTY()
	TMap<int32, class ADroneBase*> _droneMap;
#pragma endregion

	int32 _droneMode;

public:
	UStaticMeshComponent* GetOrbitalCenter()
	{
		return _orbitalCenter;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OperateDroneContainer(UStaticMeshComponent* FollowPosition);

	void GenerateDrone();

	void ChangeDroneMode();

protected:
	UFUNCTION(BlueprintCallable)
	void InitializeDroneContainer(UStaticMeshComponent* DroneCore, UStaticMeshComponent* OrbitalCenter);

	void AddOrbitalPoint(FVector& Location);

	void UpdateOrbitalPointsPositions();

	UStaticMeshComponent* CreateStaticMeshComponent(UStaticMeshComponent* Parent);
};