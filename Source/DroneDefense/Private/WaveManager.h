#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "WaveManager.generated.h"

UCLASS()
class AWaveManager : public AActor
{
    GENERATED_BODY()
    AWaveManager();

private:
    int CurrentWave;
    int MaxWaves = 5;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;


    UPROPERTY(EditAnywhere, Category = "Spawn Setting")
    TArray<TSubclassOf<AEnemyBase>> MonsterClass;

    ////////////////////////////////////////////////////////////////////////////


    UPROPERTY(EditAnywhere, Category = "Spawn Position")
    TSoftObjectPtr<AActor> NorthPos;

    UPROPERTY(EditAnywhere, Category = "Spawn Position")
    TSoftObjectPtr<AActor> SouthPos;

    UPROPERTY(EditAnywhere, Category = "Spawn Position")
    TSoftObjectPtr<AActor> EastPos;

    UPROPERTY(EditAnywhere, Category = "Spawn Position")
    TSoftObjectPtr<AActor> WestPos;

    ////////////////////////////////////////////////////////////////////////////

    UFUNCTION(BlueprintCallable, Category = "Spawn Position")
    void SpawnMonster();

    FVector SpawnPosition();

    UPROPERTY(BlueprintReadOnly, Category = "Value Check Debug")
    TMap<TSubclassOf<AActor>, int> MonsterClassValues;

    void BringMonsterValue();

    void LowStairLevel();

    UFUNCTION(BlueprintCallable, Category = "Value Check Debug")
    void PrintMonsterClassValues();

    void SpawnMonsterValueInWave();

    UFUNCTION(BlueprintCallable, Category = "Wave Control")
    void WaveStart();

    UFUNCTION(BlueprintCallable, Category = "Wave Control")
    void WaveEnd();

    UPROPERTY(BlueprintReadOnly, Category = "Wave Control")
    int MonsterNumInWave;

    TArray<TSubclassOf<AActor>> MonsterClassInWave;

    void ShakeMonsterList();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Value Control")
    int StartWaveValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Value Control")
    int MultipleWaveValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Control")
    float SpawnDelay = 1.0f;

public:
    void MonsterDeath();

};