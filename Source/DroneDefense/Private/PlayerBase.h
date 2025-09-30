// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneDefense/DroneDefenseCharacter.h"
#include "PlayerBase.generated.h"

UCLASS()
class APlayerBase : public ADroneDefenseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Drone Container")
	TSubclassOf<class ADroneContainer> DroneContainerClass;

	UPROPERTY()
	class ADroneContainer* _droneContainer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Center Option")
	TArray<struct FOrbitalCenterData> _orbitalCenterDataArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void GenerateDroneContainer();

	UFUNCTION(BlueprintCallable, Category = "Drone Container")
	void GenerateDroneInput();

	UFUNCTION(BlueprintCallable, Category = "Drone Container")
	void ChangeDroneModeInput();

	// 소켓 이름으로 컴포넌트를 찾는 함수
	UStaticMeshComponent* FindComponentBySocketName(const FName& SocketName);
};