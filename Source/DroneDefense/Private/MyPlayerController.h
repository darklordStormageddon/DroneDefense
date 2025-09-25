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

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Player Controller|Wave Manager")
	void StartPlay();
	virtual void StartPlay_Implementation();
};