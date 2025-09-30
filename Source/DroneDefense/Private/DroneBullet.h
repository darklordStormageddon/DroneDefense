// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "DroneBullet.generated.h"

UCLASS()
class ADroneBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroneBullet();

protected:
	AEnemyBase* _target = nullptr;

	FVector _targetLocation = FVector::Zero();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Bullet|Movement")
	float _moveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Bullet|Movement", meta = (UIMin = "1", UIMax = "10"))
	float _minRotateSpeed = 5.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Bullet|Movement", meta = (UIMin = "10", UIMax = "50"))
	float _maxRotateSpeed = 10.0;

	float _currentRotateSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Bullet|Movement")
	float _arriveRange = 100.0;

	float _sqrArriveRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Bullet|Attack")
	float _damage = 10.0f;

	UParticleSystemComponent* _particleOnHit = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeBullet(AEnemyBase* Target);

protected:
	UFUNCTION(BlueprintCallable)
	void CachAttackParticle(UParticleSystemComponent* OnHitParticle);

	void MoveToTarget();
};
