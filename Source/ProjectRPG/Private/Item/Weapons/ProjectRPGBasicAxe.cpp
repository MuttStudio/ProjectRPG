#include "ProjectRPG.h"
#include "ProjectRPGBasicAxe.h"

AProjectRPGBasicAxe::AProjectRPGBasicAxe(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AProjectRPGBasicAxe::Tick(float DeltaSeconds)
{
    if (!IsAttacking)
    {
        return;
    }

    FVector loc1 = EquipMesh->GetSocketLocation("Socket1");
    FVector loc2 = EquipMesh->GetSocketLocation("Socket2");

    FHitResult hit;
    FCollisionQueryParams params("WeaponTrace", true, this);

    if (GetWorld()->LineTraceSingle(hit, loc1, loc2, params, ECC_Pawn))
    {
        AActor* otherActor = hit.GetActor();

        if (PlayersHit.Contains(otherActor))
        {
            return;
        }

        PlayersHit.Add(otherActor);

        if ((otherActor != NULL) && (otherActor != this))
        {
            FPointDamageEvent pointDamage;
            // pointDamage.DamageTypeClass -- Need to make some damage types
            pointDamage.HitInfo = hit;
            pointDamage.ShotDirection = hit.ImpactNormal;
            pointDamage.Damage = BaseDamage;

            otherActor->TakeDamage(pointDamage.Damage, pointDamage, ControllingPawn->GetController(), this);
        }
        DrawDebugLine(
            GetWorld(),
            loc1,
            loc2,
            FColor(255, 0, 0),
            false,
            3,
            0,
            1
            );
    }
}
