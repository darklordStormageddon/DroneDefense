// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneBase.h"
#include "DroneContainer.h"
#include "EnemyBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADroneBase::ADroneBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADroneBase::BeginPlay()
{
	Super::BeginPlay();

	// 제곱 거리 미리 계산 (최적화)
	_sqrArriveRange = FMath::Pow(_arriveRange, 2);
	_sqrMinDistance = FMath::Pow(_minDistance, 2);
	_sqrMaxDistance = FMath::Pow(_maxDistance, 2);
	_sqrSearchTargetRange = FMath::Pow(_searchTargetRange, 2);
	_sqrAttackTracingRange = FMath::Pow(_attackTracingRange, 2);
}

// Called every frame
void ADroneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!_droneContainer)
		return;

	if (_leftAttackDelay > 0.0f)
	{
		_leftAttackDelay -= DeltaTime;
	}

	if (_droneMode != (int32)E_DroneState_Type::Tracing || !TryTracing())
	{
		RotateOrbital(DeltaTime);
	}

	Attack(DeltaTime);
}

void ADroneBase::InitializeDrone(ADroneContainer* DroneContainer)
{
	_droneContainer = DroneContainer;
}

void ADroneBase::CachAttackParticle(UParticleSystemComponent* StadardAttackParticle, UParticleSystemComponent* TracingAttackParticle, UParticleSystemComponent* HorizontalAttackParticle)
{
	_particleStandardAttack = StadardAttackParticle;
	_particleTracingAttack = TracingAttackParticle;
	_particleHorizontalAttack = HorizontalAttackParticle;

	if (_particleStandardAttack)
	{
		_particleStandardAttack->DeactivateImmediate();
	}

	if (_particleTracingAttack)
	{
		_particleTracingAttack->DeactivateImmediate();
	}

	if (_particleHorizontalAttack)
	{
		_particleHorizontalAttack->DeactivateImmediate();
	}
}

void ADroneBase::ChangeDroneMode(int32 DroneMode)
{
	_droneMode = DroneMode;
	if (_droneMode == (int32)E_DroneState_Type::Tracing)
	{
		_target = SearchTarget();
	}
}

void ADroneBase::SetOrbitalPosition(FVector TargetLocation, FRotator TargetRotator)
{
	_targetLocation = TargetLocation;
	_targetRotator = TargetRotator;
}

void ADroneBase::FireStandardAttack(AEnemyBase* TearchedTarget)
{
	if (!_bulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("FireStandardAttack: _bulletClass is null"));
		return;
	}
	
	ADroneBullet* _bullet = GetWorld()->SpawnActor<ADroneBullet>(_bulletClass.Get(), GetActorLocation(), GetActorRotation());
	if (_bullet)
	{
		_bullet->InitializeBullet(SearchTarget());
		if (_particleStandardAttack)
		{
			_particleStandardAttack->DeactivateImmediate();
			_particleStandardAttack->Activate();
		}
	}
}

AEnemyBase* ADroneBase::SearchTarget()
{
	// 적 배열 가져오기
	TArray<AActor*> _foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), _foundActors);
	
	if (_foundActors.Num() <= 0)
	{
		// 적이 없으면 타겟 초기화
		return nullptr;
	}
	
	float _closestDistanceSqr = FLT_MAX;
	AEnemyBase* _closestEnemy = nullptr;
	
	for (AActor* _actor : _foundActors)
	{
		AEnemyBase* _enemy = Cast<AEnemyBase>(_actor);
		if (!_enemy)
			continue;
			
		float _distanceSqr = FVector::DistSquared(GetActorLocation(), _enemy->GetActorLocation());
		
		if (_distanceSqr <= _sqrSearchTargetRange && _distanceSqr < _closestDistanceSqr)
		{
			_closestDistanceSqr = _distanceSqr;
			_closestEnemy = _enemy;
		}
	}
	
	return _closestEnemy;
}

bool ADroneBase::TryTracing()
{
	if (!_target.IsValid())
	{
		_target = SearchTarget();
	}

	if (!_target.IsValid())
	{
		_target = nullptr;
		return false;
	}

	FVector _currentPosition = GetActorLocation();
	FVector _targetPos = _target->GetActorLocation() + FVector(0, 0, _droneHeight);
	float _currentSqrDistance = (_targetPos - _currentPosition).SquaredLength();

	// 목표 회전 속도 계산
	float _aimRotateSpeed;
	if (_currentSqrDistance <= _sqrMinDistance)
	{
		_currentRotateSpeed = _nonRotateSpeed;
		_aimRotateSpeed = _currentRotateSpeed;
	}
	else if(_currentSqrDistance <= _sqrMaxDistance)
	{
		_aimRotateSpeed = _maxRotateSpeed;
	}
	else
	{
		_aimRotateSpeed = _minRotateSpeed;
	}

	Tracing(_targetPos, _aimRotateSpeed);

	if (_isDebugDraw)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White,
			FString::Printf(TEXT("Rotation Speed: %.2f"), _currentRotateSpeed));

		DrawDebugLine(GetWorld(), _currentPosition, _targetPos, FColor::Green, false, 0.1f, 0, 1.0f);

		float _deltaTime = GetWorld()->GetDeltaSeconds();
		DrawDebugSphere(GetWorld(), _targetPos, _minDistance, 16, FColor::Yellow, false, _deltaTime * 1.1f);
		DrawDebugSphere(GetWorld(), _targetPos, _maxDistance, 16, FColor::Yellow, false, _deltaTime * 1.1f);
	}

	return true;
}

void ADroneBase::Tracing(FVector TargetLocation, float TargetRotationSpeed)
{
	FVector _currentPosition = GetActorLocation();
	FVector _forward = GetActorForwardVector();
	float _deltaTime = GetWorld()->GetDeltaSeconds();

	// 목표 방향
	FVector _toTarget = (TargetLocation - _currentPosition).GetSafeNormal();

	// 내적 값 기반 회전 속도 변경률
	float _lerpRate = FVector::DotProduct(_forward, _toTarget);
	_lerpRate = FMath::Clamp(_lerpRate, 0.01f, 1.0f);
	_currentRotateSpeed = FMath::Lerp(_currentRotateSpeed, TargetRotationSpeed, _lerpRate);

	// 회전
	FRotator _newRotation = FMath::RInterpTo(GetActorRotation(), _toTarget.Rotation(), _deltaTime, _currentRotateSpeed);
	SetActorRotation(_newRotation);

	// 전진
	SetActorLocation(_currentPosition + _forward * _moveSpeed * _deltaTime);
}

void ADroneBase::RotateOrbital(float DeltaTime)
{
	if (_isDebugDraw)
	{
		DrawDebugSphere(GetWorld(), _targetLocation, _arriveRange, 16, FColor::Yellow, false, DeltaTime * 2.0f);
	}

	FVector _currentLocation = GetActorLocation();
	float _sqrToTargetDistance = (_targetLocation - _currentLocation).SquaredLength();

	// 목표 위치에 도달했는지 확인
	if (_sqrToTargetDistance <= _sqrArriveRange)
	{
		SetActorLocation(_targetLocation);

		FVector _forward = (_currentLocation - _droneContainer->GetOrbitalCenter()->GetComponentLocation()).GetSafeNormal();
		FVector _up = -_droneContainer->GetActorForwardVector();

		// forward와 up 벡터를 각각 향하도록 회전 설정
		FMatrix _rotationMatrix = FRotationMatrix::MakeFromXZ(_forward, _up);
		SetActorRotation(_rotationMatrix.Rotator());
		return;
	}

	Tracing(_targetLocation, _maxRotateSpeed);
}

void ADroneBase::Attack(float DeltaTime)
{
	if (_leftAttackDelay > 0.0f)
		return;
	
	float _attackRange = 0.0f;
	switch (_droneMode)
	{
	case (int32)E_DroneState_Type::Tracing:
	{
		_attackRange = _attackTracingRange;
		if (_target.IsValid())
		{
			FVector _targetPos = _target->GetActorLocation() + FVector(0, 0, _droneHeight);
			FVector _toTarget = _targetPos - GetActorLocation();
			if (_toTarget.SquaredLength() <= _sqrAttackTracingRange)
			{
				_target->TakeDamage(_attackTracingDamage);
				if (_particleTracingAttack)
				{
					_particleTracingAttack->ActivateSystem();
				}
				_leftAttackDelay = _attackTracingDelay;
			}
		}
		break;
	}
	case (int32)E_DroneState_Type::Horizontal:
	{
		_attackRange = _attackHorizontalRange;

		bool _isAttacked = false;
		TArray<AEnemyBase*> _enemieArray = SearchTarget(_attackHorizontalRange);
		for (AEnemyBase* _enemy : _enemieArray)
		{
			if (!_enemy)
				continue;

			_enemy->TakeDamage(_attackHorizontalDamage);
			if (_particleHorizontalAttack)
			{
				_particleHorizontalAttack->ActivateSystem();
			}
			_isAttacked = true;
		}
		
		if (_isAttacked)
		{
			_leftAttackDelay = _attackHorizontalDelay;
		}
		break;
	}
	}

	if (_isDebugDraw)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), _attackRange, 16, FColor::Red, false, DeltaTime * 1.01f);
	}
}

TArray<AEnemyBase*> ADroneBase::SearchTarget(float SearchRange)
{
	TArray<AEnemyBase*> _enemies;

	UWorld* _world = GetWorld();
	if (!_world)
		return _enemies;

	// Sphere 범위 내의 액터만 탐색
	TArray<FOverlapResult> _overlaps;
	FCollisionQueryParams _queryParams;
	_queryParams.AddIgnoredActor(this);

	bool _bOverlap = _world->OverlapMultiByObjectType(
		_overlaps,
		GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		FCollisionShape::MakeSphere(SearchRange),
		_queryParams
	);

	if (_bOverlap)
	{
		for (const FOverlapResult& _result : _overlaps)
		{
			AEnemyBase* _enemy = Cast<AEnemyBase>(_result.GetActor());
			if (_enemy)
			{
				_enemies.Add(_enemy);
			}
		}
	}

	return _enemies;
}