

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

    bool inMenuInputMode;

protected:
    virtual void SetupInputComponent() OVERRIDE;

};
