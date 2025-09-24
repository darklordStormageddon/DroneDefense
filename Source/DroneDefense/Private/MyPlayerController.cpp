// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

void AMyPlayerController::SetWaveManager(AWaveManager* WaveManager)
{
    _waveManager = WaveManager;
    
    if (!_waveManager)
    {
        UE_LOG(LogTemp, Error, TEXT("WaveManager is nullptr"));
        return;
    }

    // 블루프린트에서 오버라이드한 메서드를 통해 웨이브 값 가져오기
    int _startWave = GetStartWave();
    int _endWave = GetEndWave();
    
    // 블루프린트에서 구현하지 않은 경우 기본값 사용
    if (_startWave <= 0)
    {
        _startWave = 1;
    }
        
    if (_endWave <= 0)
    {
        _endWave = 10;
    }

    // 웨이브 매니저에 시작 웨이브와 종료 웨이브 값 전달
    _waveManager->InitWaveStartEnd(_startWave, _endWave);

    // 타이머 설정하여 지연 후 StartPlay 호출
    GetWorld()->GetTimerManager().SetTimer(
        _startDelayTimerHandle,
        this,
        &AMyPlayerController::StartPlay,
        _startDelay,
        false
    );
}

void AMyPlayerController::StartPlay()
{
    _waveManager->BeginWaveStart();
}