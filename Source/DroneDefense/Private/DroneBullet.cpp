// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneBullet.h"
#include "EnemyBase.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADroneBullet::ADroneBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADroneBullet::BeginPlay()
{
	Super::BeginPlay();
	
	_sqrArriveRange = FMath::Pow(_arriveRange, 2);
	_currentRotateSpeed = _minRotateSpeed;
}

// Called every frame
void ADroneBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_target)
	{
		_targetLocation = _target->GetActorLocation();
	}

	//DrawDebugSphere(GetWorld(), _targetLocation, _arriveRange, 13, FColor::Green, false, DeltaTime * 1.01f);

	MoveToTarget();
}

void ADroneBullet::InitializeBullet(AEnemyBase* Target)
{
	_target = Target;
	_targetLocation = _target ? _target->GetActorLocation() : GetActorLocation();
}

void ADroneBullet::CachAttackParticle(UParticleSystemComponent* OnHitParticle)
{
	_particleOnHit = OnHitParticle;
	if (_particleOnHit)
	{
		_particleOnHit->DeactivateImmediate();
	}
}

void ADroneBullet::MoveToTarget()
{
	FVector _currentPosition = GetActorLocation();

	// 목표 방향
	FVector _toTarget = _targetLocation - _currentPosition;
	if (_toTarget.SquaredLength() <= _sqrArriveRange)
	{
		if (_target)
		{
			_target->TakeDamage(_damage);
		}

		if (_particleOnHit)
		{
			_particleOnHit->Activate();
		}

		Destroy();
		return;
	}

	FVector _forward = GetActorForwardVector();
	float _deltaTime = GetWorld()->GetDeltaSeconds();

	// 내적 값 기반 회전 속도 변경률
	_toTarget = _toTarget.GetSafeNormal();
	float _lerpRate = FVector::DotProduct(_forward, _toTarget);
	_lerpRate = FMath::Clamp(_lerpRate, 0.01f, 1.0f);
	_currentRotateSpeed = FMath::Lerp(_currentRotateSpeed, _maxRotateSpeed, _lerpRate);

	// 회전
	FRotator _newRotation = FMath::RInterpTo(GetActorRotation(), _toTarget.Rotation(), _deltaTime, _currentRotateSpeed);
	SetActorRotation(_newRotation);

	// 전진
	SetActorLocation(_currentPosition + _forward * _moveSpeed * _deltaTime);
}