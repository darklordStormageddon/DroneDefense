// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "DroneContainer.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	GenerateDroneContainer();
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerBase::GenerateDroneContainer()
{
	if (!DroneContainerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DroneContainerClass is not set"));
		return;
	}

	// 드론 컨테이너 생성
	_droneContainer = GetWorld()->SpawnActor<ADroneContainer>(DroneContainerClass);
	_droneContainer->SetActorLocation(GetActorLocation());
	
	// 소켓 이름으로 컴포넌트를 찾아서 orbitalCenter에 할당
	for (int32 i = 0; i < _orbitalCenterDataArray.Num(); i++)
	{
		if (_orbitalCenterDataArray[i].socketName != NAME_None)
		{
			UStaticMeshComponent* _foundComponent = FindComponentBySocketName(_orbitalCenterDataArray[i].socketName);
			if (_foundComponent)
			{
				_orbitalCenterDataArray[i].orbitalCenter = _foundComponent;
			}
		}
	}
	
	// 오비털 센터 데이터 설정
	_droneContainer->OperateDroneContainer(_orbitalCenterDataArray);
}

void APlayerBase::GenerateDroneInput()
{
	_droneContainer->GenerateDrone();
}

void APlayerBase::ChangeDroneModeInput()
{
	_droneContainer->ChangeDroneMode();
}

UStaticMeshComponent* APlayerBase::FindComponentBySocketName(const FName& SocketName)
{
	TArray<UActorComponent*> _components;
	GetComponents(_components);
	
	for (UActorComponent* _component : _components)
	{
		UStaticMeshComponent* _staticMeshComponent = Cast<UStaticMeshComponent>(_component);
		if (_staticMeshComponent && _staticMeshComponent->GetName() == SocketName.ToString())
		{
			return _staticMeshComponent;
		}
	}
	
	return nullptr;
}