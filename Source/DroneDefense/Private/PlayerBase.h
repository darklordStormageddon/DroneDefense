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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Drone Container")
	void GenerateDroneContainer(UStaticMeshComponent* FollowPosition);

	UFUNCTION(BlueprintCallable, Category = "Drone Container")
	void GenerateDroneInput();

	UFUNCTION(BlueprintCallable, Category = "Drone Container")
	void ChangeDroneModeInput();
};