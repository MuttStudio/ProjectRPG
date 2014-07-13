#include "ProjectRPG.h"
#include "ProjectRPGCharacter.h"
#include "ProjectRPGLocationObjective.h"

AProjectRPGLocationObjective::AProjectRPGLocationObjective(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
}

void AProjectRPGLocationObjective::Tick(float deltaSeconds)
{
    Super::Tick(deltaSeconds);
    if (!IsActive || IsComplete)
    {
        return;
    }

    FVector CamLoc;
    FRotator CamRot;

    const FVector Direction = CamRot.Vector();
    const FVector EndTrace = TargetLocation + ValidRadius;

    FCollisionQueryParams TraceParams(FName(TEXT("LocationObjectiveTrace")), true, this);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = true;
    TraceParams.bFindInitialOverlaps = true;
    TArray<FHitResult> Hits;
    TArray<AActor*> players = TArray<AActor*>();
    if (GetWorld()->SweepMulti(Hits, TargetLocation, EndTrace, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(ValidRadius), TraceParams))
    {

        //DrawDebugSphere(GetWorld(), TargetLocation, ValidRadius, 32, FColor(255, 0, 0), false, 10.0f);
        for (FHitResult& hit : Hits)
        {
            AProjectRPGCharacter* player = Cast<AProjectRPGCharacter>(hit.GetActor());

            if (player != NULL)
            {
                SetComplete(player);
            }
        }
    }
}
