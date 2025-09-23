// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneContainer.h"
#include "DroneBase.h"

// Sets default values
ADroneContainer::ADroneContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADroneContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroneContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_droneCore->AddWorldRotation(FRotator(0.0f, -_coreRotationSpeed * DeltaTime, 0.0f));
	
	UpdateCenterPosition();

	// Orbital Rotation
	_standardAngle += _rotationSpeed * DeltaTime;
	_standardAngle = FMath::Fmod(_standardAngle, 360.0f);
	_orbitalCenter->SetRelativeRotation(FRotator(0, 0, _standardAngle));

	// Drone Location & Rotation
	FVector _forwardVector = _orbitalCenter->GetForwardVector();
	FRotator _forwardRotator = _forwardVector.Rotation();

	for (int32 i = 0; i < _droneMap.Num(); i++)
	{
		if (ADroneBase* _drone = _droneMap.FindRef(i))
		{
			_drone->SetTargetPosition(_droneOrbitalArray[i]->GetComponentLocation(), _forwardRotator);
		}
	}
}

void ADroneContainer::OperateDroneContainer(TArray<FOrbitalCenterData> OrbitalCenterDataArray)
{
	for (FOrbitalCenterData& _orbitalCenterData : OrbitalCenterDataArray)
	{
		_orbitalCenterDataMap.Add((int32)_orbitalCenterData.droneState, _orbitalCenterData);
	}

	_currentDroneMode = -1;
	ChangeDroneMode();
}

void ADroneContainer::GenerateDrone()
{
	FVector _location = FVector::ZeroVector;
	AddOrbitalPoint(_location);

	if (_droneClass)
	{
		ADroneBase* _newDrone = GetWorld()->SpawnActor<ADroneBase>(_droneClass.Get(), _location, FRotator::ZeroRotator);
		
		if (_newDrone)
		{
			_newDrone->InitializeDrone(this);
			_newDrone->ChangeTraceMode(_currentDroneMode == (int32)E_DroneState_Type::Tracing);
			
			int32 _nextIndex = _droneMap.Num();
			_droneMap.Add(_nextIndex, _newDrone);
		}
	}
}

void ADroneContainer::ChangeDroneMode()
{
	_currentDroneMode++;
	_currentDroneMode %= (int32)E_DroneState_Type::SIZE;

	// Change Orbital Center Data
	if (_currentDroneMode != (int32)E_DroneState_Type::Tracing)
	{
		FOrbitalCenterData _orbitalCenterData = _orbitalCenterDataMap.FindRef(_currentDroneMode);
		_targetOrbitalCenter = _orbitalCenterData.orbitalCenter;
		_followCenterSpeed = _orbitalCenterData.followCenterSpeed;
		_rotationSpeed = _orbitalCenterData.rotationSpeed;
		_rotationRadius = _orbitalCenterData.rotationRadius;
		UpdateOrbitalPositions();
	}

	for (int32 i = 0; i < _droneMap.Num(); i++)
	{
		if (ADroneBase* _drone = _droneMap.FindRef(i))
		{
			_drone->ChangeTraceMode(_currentDroneMode == (int32)E_DroneState_Type::Tracing);
		}
	}

	const TCHAR* _droneModeName = TEXT("Unknown");
	switch ((E_DroneState_Type)_currentDroneMode)
	{
	case E_DroneState_Type::Standard:
		_droneModeName = TEXT("Standard");
		break;
	case E_DroneState_Type::Tracing:
		_droneModeName = TEXT("Tracing");
		break;
	case E_DroneState_Type::Horizontal:
		_droneModeName = TEXT("Horizontal");
		break;
	// 필요시 다른 상태 추가
	default:
		_droneModeName = TEXT("Unknown");
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Drone Mode : %s"), _droneModeName);
}

void ADroneContainer::InitializeDroneContainer(UStaticMeshComponent* DroneCore, UStaticMeshComponent* OrbitalCenter)
{
	_droneCore = DroneCore;
	_orbitalCenter = OrbitalCenter;
}

void ADroneContainer::UpdateCenterPosition()
{
	if (!_droneCore || !_targetOrbitalCenter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateCenterPosition: _droneCore or _targetOrbitalCenter is null"));
		return;
	}

	float _lerpRate = GetWorld()->GetDeltaSeconds() * _followCenterSpeed;

	// 위치
	FVector _nextLocation = FMath::Lerp(GetActorLocation(), _targetOrbitalCenter->GetComponentLocation(), _lerpRate);
	SetActorLocation(_nextLocation);

	// 회전
	FRotator _nextRotator = _targetOrbitalCenter->GetComponentRotation();
	if (_currentDroneMode == (int32)E_DroneState_Type::Horizontal)
	{
		FVector _forwardVector = _targetOrbitalCenter->GetForwardVector();
		FVector _upVector = FVector::UpVector;
		FMatrix _rotationMatrix = FRotationMatrix::MakeFromXZ(_forwardVector, _upVector);
		_nextRotator = _rotationMatrix.Rotator();
	}

	FRotator _nextRotation = FMath::Lerp(GetActorRotation(), _nextRotator, _lerpRate);
	SetActorRotation(_nextRotation);
}

void ADroneContainer::AddOrbitalPoint(FVector& Location)
{
	Location = FVector(0, 0, 0);
	if (!_orbitalCenter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddOrbitalPoint: _centerTrans is null"));
		return;
	}

	UStaticMeshComponent* _newDroneComp = CreateStaticMeshComponent(_orbitalCenter);
	if (!_newDroneComp)
	{
		UE_LOG(LogTemp, Error, TEXT("AddOrbitalPoint: Failed to create new component"));
		return;
	}

	if (_droneOrbitalArray.Num() == 0)
	{
		// 기존 _standardTrans가 있으면 제거
		if (_orbitalStandard)
		{
			_orbitalStandard->DestroyComponent();
			_orbitalStandard = _newDroneComp;
		}
	}
	
	_droneOrbitalArray.Add(_newDroneComp);
	UpdateOrbitalPositions();
	Location = _newDroneComp->GetComponentLocation();
}

void ADroneContainer::UpdateOrbitalPositions()
{
	// 현재 궤도 드론의 개수
	int32 _droneCount = _droneOrbitalArray.Num();

	if (_droneCount <= 0)
		return;

	// 각도 간격 계산 (360도 / 개수)
	float _angleStep = 360.0f / _droneCount;

	// 모든 드론을 균등하게 배치
	for (int32 i = 0; i < _droneCount; ++i)
	{
		if (_droneOrbitalArray[i])
		{
			// 각도 계산 (인덱스 * 각도간격)
			float _droneAngle = i * _angleStep;

			// Z축 회전 (forward축을 수직으로 하는 YZ 평면에서 회전)
			float _radians = FMath::DegreesToRadians(_droneAngle);

			// 간단한 삼각함수로 위치 계산
			// cos(0) = 1, sin(0) = 0이므로 i=0일 때 (0,0,radius) 위치가 됨
			float _y = FMath::Sin(_radians) * _rotationRadius;
			float _z = FMath::Cos(_radians) * _rotationRadius;

			// 상대 위치 설정 (X=0, Y=sin(angle)*radius, Z=cos(angle)*radius)
			_droneOrbitalArray[i]->SetRelativeLocation(FVector(0, _y, _z));
		}
	}
}

UStaticMeshComponent* ADroneContainer::CreateStaticMeshComponent(UStaticMeshComponent* Parent)
{
	UStaticMeshComponent* _staticMeshComp = NewObject<UStaticMeshComponent>(this);

	if (_staticMeshComp)
	{
		// 부모 컴포넌트의 자식으로 설정
		_staticMeshComp->SetupAttachment(Parent);
		_staticMeshComp->RegisterComponent();
		return _staticMeshComp;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CreateStaticMeshComponent: Failed to create new component"));
		return nullptr;
	}
}