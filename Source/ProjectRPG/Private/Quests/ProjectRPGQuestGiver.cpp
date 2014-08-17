#include "ProjectRPG.h"
#include "ProjectRPGQuestGiver.h"

AProjectRPGQuestGiver::AProjectRPGQuestGiver(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
}

TArray<AProjectRPGQuest*> AProjectRPGQuestGiver::GetQuests()
{
    return AvailableQuests;
}

void AProjectRPGQuestGiver::BeginPlay()
{
    CharacterMovement->MaxWalkSpeed = 60;
    Health = 1000.0f;
    CharacterMovement->RotationRate.Yaw = 1;
}

float AProjectRPGQuestGiver::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
    float ActualDamage = DamageAmount;

    FHitResult hit;
    FVector direction;

    DamageEvent.GetBestHitInfo(this, DamageCauser, hit, direction);

    FString bone = hit.BoneName.ToString();

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, bone);

    FString side;
    if (bone.EndsWith("_l"))
    {
        side = "left";
    }
    else if (bone.EndsWith("_r"))
    {
        side = "right";
    }

    if (bone.Contains("thigh") || bone.Contains("calf") || bone.Contains("foot"))
    {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Leg Damage on the " + side + " side");
    }
    else if (bone.Contains("pelvis") || bone.Contains("clavicle") || bone.Contains("spine") || bone.Contains("neck"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "body damage");
    }
    else if (bone.Contains("head"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Head damage");
    }
    else if (bone.Contains("arm") || bone.Contains("hand"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Arm damage on the " + side + " side");
    }


    /*
    UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
    if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
    {
        // point damage event, pass off to helper function
        FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
        ActualDamage = InternalTakePointDamage(ActualDamage, *PointDamageEvent, EventInstigator, DamageCauser);

        // K2 notification for this actor
        if (ActualDamage != 0.f)
        {
            ReceivePointDamage(ActualDamage, DamageTypeCDO, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.ImpactNormal, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.hit.BoneName.ToString()Name, PointDamageEvent->ShotDirection, EventInstigator, DamageCauser);
            OnTakePointDamage.Broadcast(ActualDamage, EventInstigator, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.hit.BoneName.ToString()Name, PointDamageEvent->ShotDirection, DamageTypeCDO, DamageCauser);

            // Notify the component
            UPrimitiveComponent* const PrimComp = PointDamageEvent->HitInfo.Component.Get();
            if (PrimComp)
            {
                PrimComp->ReceiveComponentDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
            }
        }
    }
    else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
    {
        // radial damage event, pass off to helper function
        FRadialDamageEvent* const RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
        ActualDamage = InternalTakeRadialDamage(ActualDamage, *RadialDamageEvent, EventInstigator, DamageCauser);

        // K2 notification for this actor
        if (ActualDamage != 0.f)
        {
            FHitResult const& Hit = (RadialDamageEvent->ComponentHits.Num() > 0) ? RadialDamageEvent->ComponentHits[0] : FHitResult();
            ReceiveRadialDamage(ActualDamage, DamageTypeCDO, RadialDamageEvent->Origin, Hit, EventInstigator, DamageCauser);

            // add any desired physics impulses to our components
            for (int HitIdx = 0; HitIdx < RadialDamageEvent->ComponentHits.Num(); ++HitIdx)
            {
                FHitResult const& CompHit = RadialDamageEvent->ComponentHits[HitIdx];
                UPrimitiveComponent* const PrimComp = CompHit.Component.Get();
                if (PrimComp && PrimComp->GetOwner() == this)
                {
                    PrimComp->ReceiveComponentDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
                }
            }
        }
    }

    // generic damage notifications sent for any damage
    // note we will broadcast these for negative damage as well
    if (ActualDamage != 0.f)
    {
        ReceiveAnyDamage(ActualDamage, DamageTypeCDO, EventInstigator, DamageCauser);
        OnTakeAnyDamage.Broadcast(ActualDamage, DamageTypeCDO, EventInstigator, DamageCauser);
        if (EventInstigator != NULL)
        {
            EventInstigator->InstigatedAnyDamage(ActualDamage, DamageTypeCDO, this, DamageCauser);
        }
    }
    */

    Health -= ActualDamage;
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Ouch!: " + FString::FromInt((int)Health));
    if (Health <= 0)
    {
        Mesh->SetSimulatePhysics(true);
        Mesh->WakeAllRigidBodies();
    }
    return ActualDamage;
}
