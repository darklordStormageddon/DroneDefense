// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "EnemyBase.h"
#include "GameFramework/Actor.h"

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

    // _startWave 웨이브 시작
    StartNextWave();
}

void AMyPlayerController::OnWaveEnd(bool IsGameEnd)
{
    if (IsGameEnd)
    {
        // TODO : Result UI 호출
    }
    else
    {
        StartNextWave();
    }
}

AEnemyBase* AMyPlayerController::SearchTarget(UWorld* World, const FVector& Location, float SearchDistance)
{
    if (!World)
        return nullptr;

    // Sphere 범위 내의 액터만 탐색
    TArray<FOverlapResult> _overlaps;
    FCollisionQueryParams _queryParams;
    
    bool _bOverlap = World->OverlapMultiByObjectType(
        _overlaps,
        Location,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
        FCollisionShape::MakeSphere(SearchDistance),
        _queryParams
    );

    if (!_bOverlap)
        return nullptr;

    AEnemyBase* _closestEnemy = nullptr;
    float _closestDistanceSqr = FLT_MAX;
    for (const FOverlapResult& _result : _overlaps)
    {
        AEnemyBase* _enemy = Cast<AEnemyBase>(_result.GetActor());
        if (!_enemy || _enemy->CheckIsAlreadyDied())
            continue;

        float _distanceSqr = FVector::DistSquared(Location, _enemy->GetActorLocation());
        if (_distanceSqr <= FMath::Square(SearchDistance) && _distanceSqr < _closestDistanceSqr)
        {
            _closestDistanceSqr = _distanceSqr;
            _closestEnemy = _enemy;
        }
    }
    
    return _closestEnemy;
}

AEnemyBase* AMyPlayerController::SearchTargetFromActor(AActor* SourceActor, float SearchDistance)
{
    if (!SourceActor)
        return nullptr;
        
    UWorld* _world = SourceActor->GetWorld();
    return SearchTarget(_world, SourceActor->GetActorLocation(), SearchDistance);
}

void AMyPlayerController::OnStartWave_Implementation()
{
    _waveManager->WaveStart();
}

void AMyPlayerController::GameOver()
{
    _waveManager->SpawnEnd();
}

void AMyPlayerController::StartNextWave()
{
    // TODO : 웨이브 UI 호출

    GetWorld()->GetTimerManager().SetTimer(
        _startDelayTimerHandle,
        this,
        &AMyPlayerController::OnStartWave,
        _startDelay,
        false
    );
};