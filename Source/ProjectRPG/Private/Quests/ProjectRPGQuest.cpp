#include "ProjectRPG.h"
#include "ProjectRPGQuest.h"

AProjectRPGQuest::AProjectRPGQuest(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
}

void AProjectRPGQuest::AddOwner(AActor* owner)
{
    for (AProjectRPGQuestObjective* objective : Objectives)
    {
        objective->AddOwner(owner);
    }
}

bool AProjectRPGQuest::IsComplete()
{
    if (Objectives.Num() == 0)
    {
        return false;
    }

    for (AProjectRPGQuestObjective* objective : Objectives)
    {
        if (!objective->GetIsComplete())
        {
            return false;
        }
    }
    return true;
}

void AProjectRPGQuest::Activate()
{
    if (Objectives.Num() > 0)
    {
        for (AProjectRPGQuestObjective* objective : Objectives)
        {
            objective->SetActive();
        }
    }
}
