#include "ProjectRPG.h"
#include "ProjectRPGQuestObjective.h"

AProjectRPGQuestObjective::AProjectRPGQuestObjective(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AProjectRPGQuestObjective::SetComplete(AActor* completingPlayer)
{
    if (Owners.Contains(completingPlayer))
    {
        IsComplete = true;
        PrimaryActorTick.bCanEverTick = false;

        if (NextInLine)
        {
            NextInLine->SetActive();
        }

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ObjectiveComplete");
    }
}

void AProjectRPGQuestObjective::AddOwner(AActor* player)
{
    Owners.Add(player);
}

void AProjectRPGQuestObjective::SetActive()
{
    IsActive = true;
}

bool AProjectRPGQuestObjective::GetIsComplete()
{
    return IsComplete;
}

bool AProjectRPGQuestObjective::GetIsActive()
{
    return IsActive;
}