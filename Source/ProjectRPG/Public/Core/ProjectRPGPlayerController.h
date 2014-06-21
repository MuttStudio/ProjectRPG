

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
    virtual void ToggleMenuInputMode();

    virtual void ToggleInventoryMode();

    virtual void ToggleSpellMode();

    bool inMenuInputMode;

    bool inInventoryMode;

    bool inSpellMode;

protected:
    virtual void SetupInputComponent() OVERRIDE;

};
