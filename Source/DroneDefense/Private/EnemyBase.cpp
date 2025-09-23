#include "EnemyBase.h"
#include "WaveManager.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = false;

}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();
    HP = Max_HP;

    CalculateValue();

    if (GetWorld() && _isDestroyCount)
    {
        GetWorld()->GetTimerManager().SetTimer(
            DestroyTimerHandle,
            this,
            &AEnemyBase::DestroySelf,
            _destroyDelay,
            false
        );
    }
}

void AEnemyBase::CalculateValue()
{
    Monster_Value = Max_HP + Attack + Defense + Speed;
}

void AEnemyBase::InitializeEnemy(AWaveManager* myown)
{
    _playerController = myown;
}

void AEnemyBase::DestroySelf()
{
    if (_playerController)
    {
        _playerController->MonsterDeath();
    }

    Destroy();
}

void AEnemyBase::TakeDamage(float Damage)
{
    GotHit.Broadcast();
    HP -= Damage;
    UE_LOG(LogTemp, Warning, TEXT("HP : %d"), HP);
    if (HP <= 0)
    {
        GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyBase::DestroySelf, 2.0f, false);
    }
}

void AEnemyBase::ToggleClose()
{
     GotClose.Broadcast();
}