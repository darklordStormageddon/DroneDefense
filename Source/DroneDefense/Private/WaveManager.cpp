#include "WaveManager.h"
#include <iostream>
#include <Windows.h>
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"

AWaveManager::AWaveManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWaveManager::BeginPlay()
{
    Super::BeginPlay();

    srand((unsigned int)time(NULL));

    AMyPlayerController* _playerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!_playerController)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr"));
        return;
    }

    _playerController->SetWaveManager(this);
}

void AWaveManager::InitWaveStartEnd(int start, int end)
{
    StartWave = start;
    EndWave = end;

    CurrentWave = StartWave;
    
    UE_LOG(LogTemp, Warning, TEXT("Wave Manager Initialized - Start Wave: %d, Max Wave: %d"), StartWave, EndWave);
}

void AWaveManager::BeginWaveStart() { WaveStart(CurrentWave); }

// 웨이브 시작, 끝 함수
void AWaveManager::WaveStart(int Wave)
{
    UE_LOG(LogTemp, Warning, TEXT("현재 웨이브는 %d 웨이브입니다."), CurrentWave)
    if (SpawnCheck == false)
        WaveEnd();
    
    CurrentWave = Wave;
    
    AMyPlayerController* _playerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    
    if (_playerController)
    {
        _playerController->ChangeWave(CurrentWave);
    }

    // Monster Number Init
    TotalMonster = 0;
    SpawnMonsterAdd = 0;
    BossSpawnBool = false;

    // 현재 웨이브 값 = 웨이브 1의 값 + (현재 웨이브가 몇번째 웨이브인지 - 1) * 증가값
    WaveValue = StartWaveValue + (CurrentWave - 1) * MultipleWaveValue;
    LowWaveValue = WaveValue;
    BringMonsterValue();
    SpawnMonsterValueInWave();

    _playerController->ChangeEnemyCount(MonsterNumInWave, TotalMonster);

    SpawnMonster();
}

void AWaveManager::WaveEnd() 
{
    if (CurrentWave >= EndWave)
        SpawnEnd();
    else
    {
        CurrentWave++;
        WaveStart(CurrentWave);
    }
        
}

void AWaveManager::SpawnEnd()
{
    SpawnCheck = false;
}

void AWaveManager::GameDefeatSpawnEnd()
{
    SpawnEnd();
    WaveEnd();
}

void AWaveManager::MonsterDeath()
{
    MonsterNumInWave--;
    UE_LOG(LogTemp, Warning, TEXT("몬스터 사망, 현재 남은 몬스터 수 : %d"), MonsterNumInWave);

    AMyPlayerController* _playerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    _playerController->ChangeEnemyCount(MonsterNumInWave, TotalMonster);

    if (MonsterNumInWave <= 0)
        WaveEnd();
}

// 몬스터 Spawn, delay 설정
void AWaveManager::SpawnMonster()
{
    FTimerHandle BossTempHandle;
    FTimerDelegate BossDelegate;

    for (int index = 0; index < MonsterClassInWave.Num(); index++)
    {
        float DelayTime = index * SpawnDelay;
        FTimerHandle TempHandle;
        FTimerDelegate Delegate;

        // 지연 시간 설정: index * SpawnDelay
        
        Delegate.BindLambda([this, index]()
            {
                /*UE_LOG(LogTemp, Warning, TEXT("%d"), index)*/
                if (MonsterClassInWave.IsValidIndex(index))
                {
                    
                    FVector SpawnLoc = SpawnPosition();
                    FRotator SpawnRot = FRotator::ZeroRotator;


                    FActorSpawnParameters SpawnParams;

                    // 충돌 무시하고 무조건 스폰되도록 설정
                    SpawnParams.SpawnCollisionHandlingOverride =
                        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                    Enemy = GetWorld()->SpawnActor<AEnemyBase>(MonsterClassInWave[index], SpawnLoc, SpawnRot, SpawnParams);
                    
                    // 보스 어레이가 NULL이면 오류 뜨므로 해결 중요
                        BossSpawner();

                    SpawnMonsterAdd++;
                    /*
                    UE_LOG(LogTemp, Warning, TEXT("SpawnMonsterAdd : %d"), SpawnMonsterAdd);
                    UE_LOG(LogTemp, Warning, TEXT("TotalMonster : %d"), TotalMonster);
                    */
                    if (Enemy)
                    {
                        Enemy->InitializeEnemy(this);
                    }
                }
         });

        if (index == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("%d"), index)

            FVector SpawnLoc = SpawnPosition();
            FRotator SpawnRot = FRotator::ZeroRotator;

            FActorSpawnParameters SpawnParams;

            // 충돌 무시하고 무조건 스폰되도록 설정
            SpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            Enemy = GetWorld()->SpawnActor<AEnemyBase>(MonsterClassInWave[index], SpawnLoc, SpawnRot, SpawnParams);
            SpawnMonsterAdd++;

            //UE_LOG(LogTemp, Warning, TEXT("SpawnMonsterAdd : %d"), SpawnMonsterAdd);
            //UE_LOG(LogTemp, Warning, TEXT("TotalMonster : %d"), TotalMonster);
        }
        else
            // 타이머 설정
            GetWorld()->GetTimerManager().SetTimer(TempHandle, Delegate, DelayTime, false);

            // 타이머 설정
    }
}
  
void AWaveManager::BossSpawner()
{
    FVector SpawnLoc = SpawnPosition();
    FRotator SpawnRot = FRotator::ZeroRotator;

    int BossHave = sizeof(BossClass) / sizeof(TSubclassOf<AEnemyBase>);

    //<< --- LevelSequence Spawn Pos  
    if (CurrentWave < BossHave)
    {
        float BossSpawnLogic = (float)SpawnMonsterAdd / (float)(TotalMonster - 1) * 100.0f;

        if (BossSpawnBool == false && BossSpawnLogic >= BossSpawnPercent)
        {
            Enemy = GetWorld()->SpawnActor<AEnemyBase>(BossClass[CurrentWave - 1], SpawnLoc, SpawnRot);
            SpawnMonsterAdd++;
            UE_LOG(LogTemp, Warning, TEXT("보스 소환"));

            BossSpawnBool = true;
        }
    }
}

// Spawn 위치 반환
FVector AWaveManager::SpawnPosition()
{
    int PositionNum = rand() % 4;
    FVector ResultPos;
    switch (PositionNum)
    {
    case 0:
        ResultPos = NorthPos->GetActorLocation();
        break;
    case 1:
        ResultPos = EastPos->GetActorLocation();
        break;
    case 2:
        ResultPos = SouthPos->GetActorLocation();
        break;
    case 3:
        ResultPos = WestPos->GetActorLocation();
        break;
    }

    return ResultPos;

}


// 몬스터 Value값 가져오는 함수
void AWaveManager::BringMonsterValue()
{
    //MonsterClassValues = 몬스터 Class와 몬스터 Value를 각각 Key값과 Value값으로 담은 맵형 Map함수
    MonsterClassValues.Empty();
    MonsterClassInWave.Empty();

    // 임시로 몬스터 인스턴스를 받기 위해 맵 밖에 임시 위치
    FVector SpawnLocation = FVector(5000.f, 5000.f, 5000.f);
    //Actor클래스를 포인터로 받아들인 SpawnedActor변수 생성
    AEnemyBase* SpawnedActor;

    //Map함수에 몬스터 class를 넣기 MonsterClass.Num만큼 받아서 넣는 작업
    for (int index = 0; index < MonsterClass.Num(); index++)
    {
        SpawnedActor = GetWorld()->SpawnActor<AEnemyBase>(MonsterClass[index], SpawnLocation, FRotator::ZeroRotator);
        if (SpawnedActor)
        {
            MonsterClassCheckInWave.Add(MonsterClass[index], 0);
            MonsterClassValues.Add(MonsterClass[index], SpawnedActor->Monster_Value);
        }
        SpawnedActor->Destroy();
    }
    if(MonsterClass.Num() > 1)
        LowStairLevel();
}

// 몬스터 키값 내림 차순으로 정렬
void AWaveManager::LowStairLevel()
{
    TArray<TPair<TSubclassOf<AActor>, int>> PairArray = MonsterClassValues.Array();
    PairArray.Sort([](const TPair<TSubclassOf<AActor>, int>& A, const TPair<TSubclassOf<AActor>, int>& B)
        {
            return A.Value < B.Value; // 오름차순 정렬
        });
    MonsterClassValues.Empty();
    for (const auto& Pair : PairArray)
    {
        MonsterClassCheckInWave.Add(Pair.Key, 0);
        MonsterClassValues.Add(Pair.Key, Pair.Value);
    }
}


//현재 웨이브 값에맞 맞게 몬스터 소환 가능한 몬스터 양을 계산해서 클래스 별 몬스터를 몇 마리 뽑을 지 결정(랜덤 숫자 x 몬스터 반환값 만큼의 소환), 뽑 반환 X, 반환 값을 몬스터
//랜덤한 몬스터 소환 가능한 웨이브 값으로 계산, 뽑 몬스터 소환 가능한 그래서 몬스터 소환 가능한 만큼만 뽑
//이런식으로 웨이브 값이 0이 되거나 뽑 수 있는 몬스터가 없으면 반복
void AWaveManager::SpawnMonsterValueInWave()
{
    int NonSpawnCheck = 0;
    
    TotalMonster++;//Boss
    
    MonsterClassInWave.Empty();

    // 키(몬스터 클래스) 값들을 Keys라는 배열 형태로 가져 오기
    TArray<TSubclassOf<AActor>> Keys;

    //MonsterClassValues의 반환 시킬 키(몬스터 클래스)를 넣기
    MonsterClassValues.GetKeys(Keys);

    int FullMaxCheck = 0;

    // WaveValue가 0이거나 뽑 이상 뽑 수 있는 몬스터가 없으면 반복
    while (WaveValue > 0)
    {
        bool bAnyAddedThisPass = false;    // 이번 패스에서 몬스터를 추가했는지

        // 값이 큰순으로 몬스터를 뽑 수 있는지 스캔
        for (int i = 0; i < Keys.Num(); ++i)
        {
            // Class변수 MonsterClassKey에 Keys[]를 초기화
            TSubclassOf<AActor> MonsterClassKey = Keys[i];

            // MonsterValueInWave에 각 Class의 값으로 넣어줌
            int MonsterValueInWave = MonsterClassValues[MonsterClassKey];

            // 넣어준 키가 NULL값, 즉 값이 없이면 이 객체 처리를 스킵하고 다음 키로 넘어감
            if (MonsterValueInWave <= 0) continue;

            // 해당 몬스터로 뽑 가능한지 계산하기 위해 웨이브의 값 / 몬스터 뽑 가능 값으로 계산
            int DecideVal = LowWaveValue / MonsterValueInWave;

            // 넣어준 값이 0보다 작거나 같으면 뽑 가능해도 되는 상황이면 이 객체 처리를 스킵하고 다음 키로 넘어감
            if (DecideVal <= 0) continue;

            FString Class = Keys[i]->GetName();

            // 0 ~ DecideVal 사이의 랜덤값을 뽑기 (0도 포함)
            int RandomVal = rand() % (DecideVal + 1);

            int checkNum = RandomVal + MonsterClassCheckInWave[Keys[i]];
            //UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *Class, MonsterClassCheckInWave[Keys[i]]);
            if (SpawnMaxCount(Keys[i]) < checkNum) continue;


            if (SpawnMaxCount(Keys[i]) <= MonsterClassCheckInWave[Keys[i]])
                FullMaxCheck++;

            // 그렇게 몬스터 소환할 개수, 랜덤한 숫자 뽑 MonsterClassInWave에 넣는다
            for (int j = 0; j < RandomVal; ++j)
            {
                MonsterClassInWave.Add(MonsterClassKey);
                MonsterClassCheckInWave[Keys[i]]++;
                bAnyAddedThisPass = true;
            }
            LowWaveValue -= RandomVal * MonsterValueInWave;
            if (FullMaxCheck == Keys.Num())
                break;
        }

        TSubclassOf<AActor> MonsterKey = Keys[Keys.Num() - 1];
        if (LowWaveValue < MonsterClassValues[MonsterKey])
        {
            break;
        }

        if (bAnyAddedThisPass == false)
        {
            NonSpawnCheck++;
            if (NonSpawnCheck >= 100)
                break;
        }
    }
    MonsterNumInWave = TotalMonster = MonsterClassInWave.Num();
    ShakeMonsterList(); // 몬스터 리스트 섞기
}

int AWaveManager::SpawnMaxCount(TSubclassOf<AActor> MaxValueClass)
{
    int Max = WaveValue / MonsterClassValues[MaxValueClass] / 2;
    //UE_LOG(LogTemp, Warning, TEXT("%d"), Max);
    return Max;
}
// Fisher-Yates 알고리즘으로 랜덤하게 섞기
void AWaveManager::ShakeMonsterList()
{
    int Num = MonsterClassInWave.Num();
    int MonsterValueLog = 0;
    for (int i = Num - 1; i > 0; --i)
    {
        int j = FMath::RandRange(0, i);
        MonsterClassInWave.Swap(i, j);
    }
    TArray<TSubclassOf<AActor>> Keys;
    MonsterClassCheckInWave.GetKeys(Keys);

    // 디버그 로그: 섞인 결과
    for (int i = 0; i < MonsterClassCheckInWave.Num(); ++i)
    {
        if (MonsterClassCheckInWave[Keys[i]])
        {
            FString ClassName = Keys[i]->GetName();
            UE_LOG(LogTemp, Warning, TEXT("MonsterClassCheckInWave [%s] : %d"), *ClassName, MonsterClassCheckInWave[Keys[i]]);
        }
    }
}


// 몬스터 클래스별 값을 출력해서 테스트 하는 함수(디버깅용)
void AWaveManager::PrintMonsterClassValues()
{
    for (auto& Elem : MonsterClassValues)
    {
        UClass* MonsterUClass = Elem.Key; // tsubclassof -> uclass*
        int value = Elem.Value;

        if (MonsterUClass)
        {
            FString classname = MonsterUClass->GetName(); // UClass* -> FString
            UE_LOG(LogTemp, Warning, TEXT("monster class: %s, value: %d"), *classname, value);

            //   화면에 출력
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, classname + " value: " + FString::FromInt(value));
            }
        }
    }
    
}