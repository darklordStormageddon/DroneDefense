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

USTRUCT(BlueprintType)
struct FOrbitalCenterData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	E_DroneState_Type droneState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName socketName;

	UPROPERTY()
	UStaticMeshComponent* orbitalCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float rotationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float rotationRadius;

	FOrbitalCenterData()
		: droneState(E_DroneState_Type::Standard)
		, socketName(NAME_None)
		, orbitalCenter(nullptr)
		, rotationSpeed(10.0f)
		, rotationRadius(10.0f)
	{
	}
};

UCLASS()
class ADroneContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroneContainer();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Debug")
	bool _isDebugDraw = false;

#pragma region Drone Core
	UPROPERTY()
	UStaticMeshComponent* _droneCore = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Drone Core")
	float _coreRotationSpeed = 10.0f;
#pragma endregion

#pragma region Center
	UPROPERTY()
	UStaticMeshComponent* _orbitalCenter = nullptr;

	class UStaticMeshComponent* _targetOrbitalCenter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Center", meta = (UIMin = "0.0", UIMax = "1.0"))
	float _centerFollowSpeedRate = 0.5f;

	TMap<int32, FOrbitalCenterData> _orbitalCenterDataMap = TMap<int32, FOrbitalCenterData>();
#pragma endregion

#pragma region Orbital
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Drone")
	int _maxDroneCount = 12.0f;

	int32 _currentDroneMode = 0;

	UPROPERTY()
	float _standardAngle = 0.0f;

	UPROPERTY()
	TArray<UStaticMeshComponent*> _droneOrbitalArray;

	UPROPERTY()
	UStaticMeshComponent* _orbitalStandard = nullptr;

	float _rotationSpeed = 10.0f;

	float _rotationRadius = 10.0f;
#pragma endregion

#pragma region Drone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Drone")
	TSubclassOf<class ADroneBase> _droneClass = nullptr;

	UPROPERTY()
	TMap<int32, class ADroneBase*> _droneMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Drone|Standard Attack")
	float _standardAttackRange = 300.0f;

	float _sqrStandardAttackRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Drone|Standard Attack")
	float _fireCycleInterval = 1.0f;

	float _leftCycleInterval = 0.0f;
#pragma endregion

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

	void OperateDroneContainer(TArray<FOrbitalCenterData> OrtibalCenterDataArray);

	void GenerateDrone();

	void ChangeDroneMode();

protected:
	UFUNCTION(BlueprintCallable)
	void InitializeDroneContainer(UStaticMeshComponent* DroneCore, UStaticMeshComponent* OrbitalCenter);

	void UpdateCenterPosition();

	void AddOrbitalPoint(FVector& Location);

	void UpdateOrbitalPositions();

	UStaticMeshComponent* CreateStaticMeshComponent(UStaticMeshComponent* Parent);

	void FireStandardAttack(class AEnemyBase* SearchedTarget);

	class AEnemyBase* SearchTarget();
};