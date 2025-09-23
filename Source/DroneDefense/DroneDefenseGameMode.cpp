// Copyright Epic Games, Inc. All Rights Reserved.

#include "DroneDefenseGameMode.h"
#include "DroneDefenseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADroneDefenseGameMode::ADroneDefenseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
