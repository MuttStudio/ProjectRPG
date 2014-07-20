#include "ProjectRPG.h"
#include "ProjectRPGPlayerController.h"
#include "ProjectRPGHUD.h"

AProjectRPGPlayerController::AProjectRPGPlayerController(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    inMenuInputMode = false;
    inInventoryMode = false;
    inSpellMode = false;
    inLogMode = false;
    inQuestGiveMode = false;
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
    InputComponent->BindAction("Log", IE_Pressed, this, &AProjectRPGPlayerController::ToggleLogMode);
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
    if (inQuestGiveMode)
        ToggleQuestGiveMode();
    if (inLogMode)
        ToggleLogMode();

    inMenuInputMode = !inMenuInputMode;

    ToggleInput(inMenuInputMode);

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
    if (inQuestGiveMode)
        ToggleQuestGiveMode();
    if (inLogMode)
        ToggleLogMode();

    inInventoryMode = !inInventoryMode;

    ToggleInput(inInventoryMode);

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
    if (inQuestGiveMode)
        ToggleQuestGiveMode();
    if (inLogMode)
        ToggleLogMode();

    inSpellMode = !inSpellMode;

    ToggleInput(inSpellMode);

    Cast<AProjectRPGHUD>(MyHUD)->ShowSpells(inSpellMode);
}

void AProjectRPGPlayerController::ToggleLogMode()
{
#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Controller: Toggle Spell Mode");
#endif

    // reset other modes. This lets us change around from menue to menue without going back to main
    if (inMenuInputMode)
        ToggleMenuInputMode();
    if (inInventoryMode)
        ToggleInventoryMode();
    if (inQuestGiveMode)
        ToggleQuestGiveMode();
    if (inSpellMode)
        ToggleSpellMode();

    inLogMode = !inLogMode;

    ToggleInput(inLogMode);

        Cast<AProjectRPGHUD>(MyHUD)->ShowLog(inLogMode);
}

void AProjectRPGPlayerController::ToggleQuestGiveMode()
{
#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Controller: Toggle Quest Give Mode");
#endif

    // reset other modes. This lets us change around from menue to menue without going back to main
    if (inMenuInputMode)
        ToggleMenuInputMode();
    if (inInventoryMode)
        ToggleInventoryMode();
    if (inSpellMode)
        ToggleSpellMode();
    if (inLogMode)
        ToggleLogMode();

    inQuestGiveMode = !inQuestGiveMode;

    // Special case for this mode. We need to exit the view then give control back since this one is click to exit
    Cast<AProjectRPGHUD>(MyHUD)->ShowQuestGive(inQuestGiveMode);

    if (!inQuestGiveMode)
        FPlatformProcess::Sleep(.125);

    ToggleInput(inQuestGiveMode);
}

void AProjectRPGPlayerController::ToggleInput(bool toggle)
{
    InputComponent->bBlockInput = toggle;
    bEnableClickEvents = toggle;
    bEnableMouseOverEvents = toggle;
    bShowMouseCursor = toggle;
    SetIgnoreLookInput(toggle);
    SetIgnoreMoveInput(toggle);
}