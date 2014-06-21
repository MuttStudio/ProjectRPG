#pragma once

#include "ProjectRPGPlaceable.h"
#include "ProjectRPGSpell.generated.h"

UCLASS()
class AProjectRPGSpell : public AProjectRPGPlaceable
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
        int32 CooldownTimer;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spell)
        TSubobjectPtr<class UStaticMeshComponent> CastMesh;

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Spell: Cast"))
        virtual void CastSpell();
};
