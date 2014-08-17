#pragma once

#include "Item/ProjectRPGWeapon.h"
#include "ProjectRPGBasicAxe.generated.h"

UCLASS()
class PROJECTRPG_API AProjectRPGBasicAxe : public AProjectRPGWeapon
{
    GENERATED_UCLASS_BODY()

public:
    //UFUNCTION()
    //    void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    //UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
    //    TSubobjectPtr<UBoxComponent> CollisionComp;

    void Tick(float DeltaSeconds) override;
};
