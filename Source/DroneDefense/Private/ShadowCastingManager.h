// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShadowCastingManager.generated.h"

class UStaticMeshComponent;

USTRUCT()
struct FShadowCastingData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;

	float distance = std::numeric_limits<float>::infinity();

	bool bIsCastingShadow = true;
};

UCLASS()
class AShadowCastingManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShadowCastingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//cast on threshold distance
	UPROPERTY(EditAnywhere, Category = "Shadow LOD")
	float ShadowEnableDistance = 3000.0f;

	//cast off threshold distance
	UPROPERTY(EditAnywhere, Category = "Shadow LOD")
	float ShadowDisableDistance = 3500.0f;

	//check interval
	UPROPERTY(EditAnywhere, Category = "Shadow LOD")
	float UpdateInterval = 0.25f;

	//Static mesh components to manage
	UPROPERTY(EditAnywhere, Category = "Shadow LOD")
	TArray<FShadowCastingData> ShadowTargets;

	float UpdateTimer = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Shadow LOD")
	void UpdateShadowCasting();

	UFUNCTION(BlueprintCallable, Category = "Shadow LOD")
	float GetDistanceSquaredToCamera(UStaticMeshComponent* MeshComponent);

	void CollectShadowTargets();
};

