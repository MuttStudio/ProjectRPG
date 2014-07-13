

#pragma once

#include "GameFramework/PlayerController.h"
#include "ProjectRPGPlayerController.generated.h"

/**
 *
 */
UCLASS()
class AProjectRPGPlayerController : public APlayerController
{
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Toggle Menu", CompactNodeTitle = "TglMenu", Keywords = "Toggle Menu"), Category = HUD)
    virtual void ToggleMenuInputMode();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Toggle Inventory", CompactNodeTitle = "TglInv", Keywords = "Toggle Inventory"), Category = HUD)
    virtual void ToggleInventoryMode();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Toggle Spell", CompactNodeTitle = "TglSpell", Keywords = "Toggle Spell"), Category = HUD)
    virtual void ToggleSpellMode();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Toggle Quest", CompactNodeTitle = "TglQst", Keywords = "Toggle Quest"), Category = HUD)
    virtual void ToggleQuestGiveMode();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Toggle Input", CompactNodeTitle = "TglInpt", Keywords = "Toggle Input"), Category = HUD)
    void ToggleInput(bool toggle);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Toggle Log", CompactNodeTitle = "TglLog", Keywords = "Toggle Log"), Category = HUD)
    void ToggleLogMode();

    bool inMenuInputMode;

    bool inLogMode;

    bool inInventoryMode;

    bool inSpellMode;

    bool inQuestGiveMode;

protected:
    virtual void SetupInputComponent() override;

};
