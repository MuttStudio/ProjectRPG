// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ProjectRPG.h"
#include "ProjectRPGGameMode.h"
#include "ProjectRPGHUD.h"
#include "ProjectRPGPlayerController.h"

AProjectRPGGameMode::AProjectRPGGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("/Game/Blueprints/MyCharacter"));
    if (PlayerPawnObject.Object != NULL)
    {
        DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
    }

    // use our custom HUD class
    HUDClass = AProjectRPGHUD::StaticClass();

    PlayerControllerClass = AProjectRPGPlayerController::StaticClass();
}
