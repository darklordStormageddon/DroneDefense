// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h" // FTimerHandle
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGotClose);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGotHit);

// 전방 선언
class AWaveManager;

UCLASS()
class AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();
	AWaveManager* _playerController = nullptr;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Debug")
	bool _isDestroyCount = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Debug")
	float _destroyDelay = 5.0f;

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Stats")
	float Max_HP;

	UPROPERTY(BlueprintReadWrite, Category = "Monster|Stats")
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Stats")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Stats")
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Stats")
	float Speed;


	// 몬스터 Value
	UPROPERTY(BlueprintReadOnly, Category = "Monster|Stats")
	float Monster_Value;

	void CalculateValue();

	//------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Events")
	bool Close;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Events")
	bool Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Events")
	bool Dead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Events")
	float Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Events")
	bool BossWait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Events", meta = (ExposeOnSpawn = true))
	AActor* Nexus;

	UFUNCTION(BlueprintCallable, Category = "Monster|Events")
	void InitializeEnemy(AWaveManager* myown);

	UFUNCTION(BlueprintCallable, Category = "Monster|Events")
	void DestroySelf();

	UPROPERTY(BlueprintAssignable, Category = "Monster|Events")
	FGotClose GotClose;

	UPROPERTY(BlueprintAssignable, Category = "Monster|Events")
	FGotHit GotHit;

	UFUNCTION(BlueprintCallable, Category = "Monster|Events")
	void ToggleClose();

	UFUNCTION(BlueprintCallable, Category = "Monster|Events")
	void ToggleHit();

private:
	FTimerHandle DestroyTimerHandle;

	AWaveManager* WaveManagerActor = nullptr;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Monster|Events")
	bool TakeDamage(float Damage);
	virtual bool TakeDamage_Implementation(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Monster|Events")
	float SpawnWait();

};