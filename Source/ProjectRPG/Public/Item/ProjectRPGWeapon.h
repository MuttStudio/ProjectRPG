#pragma once

#include "ProjectRPGItem.h"
#include "ProjectRPGWeapon.generated.h"

UCLASS()
class AProjectRPGWeapon : public AProjectRPGItem
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Attack)
        bool IsAttacking;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack)
        TArray<AActor*> PlayersHit;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack)
        int8 BaseDamage;

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "DoneAttacking", CompactNodeTitle = "DnAtck", Keywords = "Done Attacking"), Category = Attack)
        void DoneAttacking();
};
