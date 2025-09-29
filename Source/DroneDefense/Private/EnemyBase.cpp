#include "EnemyBase.h"
#include "WaveManager.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "WaveManager.h"


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

    //레벨에서 WaveManager 액터 찾기
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        WaveManagerActor = Cast<AWaveManager>(FoundActors[0]); // 첫 번째 액터 사용
    }
}

void AEnemyBase::CalculateValue()
{
    Monster_Value = Max_HP + Attack + Defense*5 + Speed/100;
}

void AEnemyBase::InitializeEnemy(AWaveManager* myown)
{
    _playerController = myown;
}

void AEnemyBase::DestroySelf()
{
    // MonsterDeath 호출
    if (WaveManagerActor)
    {
        WaveManagerActor->MonsterDeath();
    }

    if (!GetWorldTimerManager().IsTimerActive(DestroyTimerHandle))
        GetWorldTimerManager().SetTimer(DestroyTimerHandle, [this]()
            {
                Destroy();
            }, 2.0f, false);
}

bool AEnemyBase::TakeDamage_Implementation(float Damage)
{
    if (HP <= 0)
        return false;

    if (Damage > Defense)
    {
        HP -= Damage - Defense;
    }

    if (HP <= 0)
    {

        DestroySelf();
    }

    return true;
}

void AEnemyBase::ToggleClose()
{
     GotClose.Broadcast();
}

void AEnemyBase::ToggleHit()
{
    GotHit.Broadcast();
}

float AEnemyBase::SpawnWait()
{
    BossWait = true;
    Speed = 0;
    Timer = 0;
    return 5.0f;
}