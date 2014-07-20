#pragma once

#include "GameFramework/HUD.h"
#include "ProjectRPGHUD.generated.h"

UCLASS()
class AProjectRPGHUD : public AHUD
{
    GENERATED_UCLASS_BODY()

public:

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "HUD: Show Menu"))
        void ShowMenu(bool show);

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "HUD: Show Main Hud"))
        void ShowMainHud(bool show);

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "HUD: Show Inventory"))
        void ShowInventory(bool show);

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "HUD: Show Spells"))
        void ShowSpells(bool show);

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "HUD: Show Quest Give"))
        void ShowQuestGive(bool show);

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "HUD: Show Log"))
        void ShowLog(bool show);
};
