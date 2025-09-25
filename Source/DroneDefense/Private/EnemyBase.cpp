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
        UE_LOG(LogTemp, Display, TEXT("Found Wave manager"));
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
    
    //GotHit.Broadcast();
    if (HP <= 0) return false;

    HP -= Damage;
    if (HP <= 0)
    {
        Dead = true;

        DestroySelf();
    }
    /*else
    {
        Hit = true;//피격 시
        if (MonsterWidget)
            MonsterWidget->SetVisibility(false);//위젯 컴포넌트 숨기기
        GetWorldTimerManager().SetTimer(
            HitTimerHandle,
            [this]()   // 2초 뒤 실행
            {
                if (MonsterWidget)
                    MonsterWidget->SetVisibility(true);//위젯 컴포넌트 보이기
            },
            2.0f,
            false
        );
    }*/
    return true;
}

void AEnemyBase::ToggleClose()
{
     GotClose.Broadcast();
}

void AEnemyBase::ToggleHit()
{
    GotHit.Broadcast();
};