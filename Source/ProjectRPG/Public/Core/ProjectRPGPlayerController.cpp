#include "ProjectRPG.h"
#include "ProjectRPGPlayerController.h"
#include "ProjectRPGHUD.h"

AProjectRPGPlayerController::AProjectRPGPlayerController(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    inMenuInputMode = false;
}

void AProjectRPGPlayerController::SetupInputComponent()
{
    // A subclass could create a different InputComponent class but still want the default bindings
    if (InputComponent == NULL)
    {
        InputComponent = ConstructObject<UInputComponent>(UInputComponent::StaticClass(), this, TEXT("PC_InputComponent0"));
        InputComponent->RegisterComponent();
    }

    // Only do this if this actor is of a blueprint class
    UBlueprintGeneratedClass* BGClass = Cast<UBlueprintGeneratedClass>(GetClass());
    if (BGClass != NULL)
    {
        InputComponent->bBlockInput = bBlockInput;
        UInputDelegateBinding::BindInputDelegates(BGClass, InputComponent);
    }

    InputComponent->BindAction("Menu", IE_Pressed, this, &AProjectRPGPlayerController::ToggleMenuInputMode);
}

void AProjectRPGPlayerController::ToggleMenuInputMode()
{
#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Controller: Toggle Menu Input Mode");
#endif

    inMenuInputMode = !inMenuInputMode;

    InputComponent->bBlockInput = inMenuInputMode;
    bEnableClickEvents = inMenuInputMode;
    bEnableMouseOverEvents = inMenuInputMode;
    bShowMouseCursor = inMenuInputMode;
    SetIgnoreLookInput(inMenuInputMode);
    SetIgnoreMoveInput(inMenuInputMode);

    Cast<AProjectRPGHUD>(MyHUD)->ShowMenu(inMenuInputMode);
    Cast<AProjectRPGHUD>(MyHUD)->ShowMainHud(!inMenuInputMode);
}
