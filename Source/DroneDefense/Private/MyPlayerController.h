// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WaveManager.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

private :
	UFUNCTION(BlueprintCallable, Category = "Game End")
	void GameOver();
protected:
	AWaveManager* _waveManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Controller|Wave Manager")
	float _startDelay = 3.0f;

	FTimerHandle _startDelayTimerHandle;
	
public:
	void SetWaveManager(AWaveManager* WaveManager);

	// 블루프린트에서만 오버라이드할 수 있는 함수들
	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller|Wave Manager")
	int GetStartWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller|Wave Manager")
	int GetEndWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller|Wave Manager")
	void ChangeWave(int NextWave);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller|Wave Manager")
	void ChangeEnemyCount(int LeftEnemyCount, int TotalEnemyCount);

	void OnWaveEnd(bool IsGameEnd);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller|Wave Manager")
	float GetBossWarningDelay();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller|Wave Manager")
	void OnBossAppeared();

	UFUNCTION(Blueprintcallable, BlueprintNativeEvent, Category = "Player Controller|Wave Manager")
	void OnBossIntroFinished();
	virtual void OnBossIntroFinished_Implementation();

	/**
	 * 가장 가까운 적을 찾는 함수
	 * @param World - 현재 월드 포인터
	 * @param Location - 검색 시작 위치
	 * @param SearchDistance - 검색 범위 (반경)
	 * @return 가장 가까운 적 또는 nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Search Target")
	static AEnemyBase* SearchTarget(UWorld* World, const FVector& Location, float SearchDistance);

	/**
	 * 특정 액터에서 가장 가까운 적을 찾는 함수
	 * @param SourceActor - 검색 시작 액터
	 * @param SearchDistance - 검색 범위 (반경)
	 * @return 가장 가까운 적 또는 nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Search Target")
	static AEnemyBase* SearchTargetFromActor(AActor* SourceActor, float SearchDistance);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Player Controller|Wave Manager")
	void OnStartWave();
	virtual void OnStartWave_Implementation();

	void StartNextWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller|Wave Manager")
	void OnGameClear();
};