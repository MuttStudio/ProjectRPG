// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ProjectRPG.h"
#include "ProjectRPGGameMode.h"
#include "ProjectRPGPlayerController.h"

AProjectRPGGameMode::AProjectRPGGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("/Game/Blueprints/Player/MyCharacter"));
    if (PlayerPawnObject.Object != NULL)
    {
        DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> HUD(TEXT("/Game/Blueprints/BP_ProjectRPGHUD"));
    if (HUD.Object != NULL)
    {
        HUDClass = (UClass*)HUD.Object->GeneratedClass;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerController(TEXT("/Game/Blueprints/Player/BP_ProjectRPGPlayerController"));
    if (PlayerController.Object != NULL)
    {
        PlayerControllerClass = (UClass*)PlayerController.Object->GeneratedClass;
    }
}
