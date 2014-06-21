#include "ProjectRPG.h"
#include "ProjectRPGPlayerController.h"
#include "ProjectRPGHUD.h"

AProjectRPGPlayerController::AProjectRPGPlayerController(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    inMenuInputMode = false;
    inInventoryMode = false;
    inSpellMode = false;
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

    InputComponent->BindAction("Inventory", IE_Pressed, this, &AProjectRPGPlayerController::ToggleInventoryMode);
    InputComponent->BindAction("Menu", IE_Pressed, this, &AProjectRPGPlayerController::ToggleMenuInputMode);
    InputComponent->BindAction("Spells", IE_Pressed, this, &AProjectRPGPlayerController::ToggleSpellMode);
}

void AProjectRPGPlayerController::ToggleMenuInputMode()
{
#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Controller: Toggle Menu Input Mode");
#endif

// reset other modes. This lets us change around from menue to menue without going back to main
    if (inInventoryMode)
        ToggleInventoryMode();
    if (inSpellMode)
        ToggleSpellMode();

    inMenuInputMode = !inMenuInputMode;

    InputComponent->bBlockInput = inMenuInputMode;
    bEnableClickEvents = inMenuInputMode;
    bEnableMouseOverEvents = inMenuInputMode;
    bShowMouseCursor = inMenuInputMode;
    SetIgnoreLookInput(inMenuInputMode);
    SetIgnoreMoveInput(inMenuInputMode);

    Cast<AProjectRPGHUD>(MyHUD)->ShowMenu(inMenuInputMode);
}

void AProjectRPGPlayerController::ToggleInventoryMode()
{
#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Controller: Toggle Inventory Mode");
#endif

// reset other modes. This lets us change around from menue to menue without going back to main
    if (inMenuInputMode)
        ToggleMenuInputMode();
    if (inSpellMode)
        ToggleSpellMode();

    inInventoryMode = !inInventoryMode;

    InputComponent->bBlockInput = inInventoryMode;
    bEnableClickEvents = inInventoryMode;
    bEnableMouseOverEvents = inInventoryMode;
    bShowMouseCursor = inInventoryMode;
    SetIgnoreLookInput(inInventoryMode);
    SetIgnoreMoveInput(inInventoryMode);

    Cast<AProjectRPGHUD>(MyHUD)->ShowInventory(inInventoryMode);
}

void AProjectRPGPlayerController::ToggleSpellMode()
{
#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Controller: Toggle Spell Mode");
#endif

// reset other modes. This lets us change around from menue to menue without going back to main
    if (inMenuInputMode)
        ToggleMenuInputMode();
    if (inInventoryMode)
        ToggleInventoryMode();

    inSpellMode = !inSpellMode;

    InputComponent->bBlockInput = inSpellMode;
    bEnableClickEvents = inSpellMode;
    bEnableMouseOverEvents = inSpellMode;
    bShowMouseCursor = inSpellMode;
    SetIgnoreLookInput(inSpellMode);
    SetIgnoreMoveInput(inSpellMode);

    Cast<AProjectRPGHUD>(MyHUD)->ShowSpells(inSpellMode);
}