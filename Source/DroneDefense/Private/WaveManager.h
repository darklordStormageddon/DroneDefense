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
    AEnemyBase* Enemy;

    int CurrentWave = StartWave;
    int TotalMonster;
    int SpawnMonsterAdd;

    bool BossSpawnBool = false;

    UPROPERTY(EditAnywhere, Category = "Boss Spawn Setting")
    int BossSpawnPercent;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Spawn Setting")
    TArray<TSubclassOf<AEnemyBase>> MonsterClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Array")
    TArray<TSubclassOf<AEnemyBase>> BossClass;

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

    //int SpawnMaxCount(TSubclassOf<AActor> MaxValueClass);

    FVector SpawnPosition();

    UPROPERTY(BlueprintReadOnly, Category = "Value Check Debug")
    TMap<TSubclassOf<AActor>, int> MonsterClassValues;
    TMap<TSubclassOf<AActor>, int> MonsterClassCheckInWave;
    //TMap<TSubclassOf<AActor>, int> MonsterClassKill;

    void BringMonsterValue();

    void LowStairLevel();

    UFUNCTION(BlueprintCallable, Category = "Value Check Debug")
    void PrintMonsterClassValues();

    void SpawnMonsterValueInWave();

    UFUNCTION(BlueprintCallable, Category = "Wave Control")
    void WaveStart(int Wave);

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

private:
    UFUNCTION()
    void BossSpawner();

    int WaveValue;
    int LowWaveValue;

    bool SpawnCheck = true;
public:
    void MonsterDeath();

    int getCurrentWave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Control")
    int StartWave = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Control")
    int EndWave = 10;

    UPROPERTY(BlueprintReadOnly, Category = "Wave Control")
    int BossPercentUI = BossSpawnPercent;

    void WaveStart();

    void InitWaveStartEnd(int start, int end);

    void SpawnEnd();

    void GameDefeatSpawnEnd();

};