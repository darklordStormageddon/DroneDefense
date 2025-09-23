// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "DroneBase.generated.h"

UCLASS()
class ADroneBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroneBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Debug")
	bool _isDebugDraw = false;

	class ADroneContainer* _droneContainer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Move")
	float _moveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Move")
	float _arriveRange = 0.5f;

	float _sqrArriveRange = 0.0f;

	bool _isTraceMode = false;

	FVector _targetLocation = FVector(0, 0, 0);
	FRotator _targetRotator = FRotator(0, 0, 0);

#pragma region Trace
	TWeakObjectPtr<AEnemyBase> _target = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Trace")
	float _searchTargetRange = 1000.0f;

	float _sqrSearchTargetRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Trace")
	float _droneHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Trace", meta = (UIMin = "0.0001", UIMax = "0.01"))
	float _nonRotateSpeed = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Trace", meta = (UIMin = "0.01", UIMax = "5"))
	float _minRotateSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Trace", meta = (UIMin = "5", UIMax = "20"))
	float _maxRotateSpeed = 10.0f;

	float _currentRotateSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Trace")
	float _minDistance = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Trace")
	float _maxDistance = 300.0f;

	float _sqrMinDistance = 0.0f;
	float _sqrMaxDistance = 0.0f;
#pragma endregion

#pragma region Attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Attack|Tracing")
	float _attackTracingRange = 100.0f;

	float _sqrAttackTracingRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Attack|Tracing")
	float _attackTracingDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone|Attack|Tracing")
	float _attackTracingDelay = 1.0f;

	float _leftAttackDelay = 0.0f;
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeDrone(ADroneContainer* DroneContainer);

	void ChangeTraceMode(bool IsTraceMode);

	void SetTargetPosition(FVector TargetLocation, FRotator TargetRotator);

protected:
	void SearchTarget();

	bool TryTracing();

	void Tracing(FVector TargetLocation, float TargetRotationSpeed);

	void RotateOrbital(float DeltaTime);

	void Attack(float DeltaTime);
};