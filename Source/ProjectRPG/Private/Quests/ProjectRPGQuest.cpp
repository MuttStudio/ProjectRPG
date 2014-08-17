#include "ProjectRPG.h"
#include "ProjectRPGQuest.h"
#include "UnrealNetwork.h"

AProjectRPGQuest::AProjectRPGQuest(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    SetReplicates(true);
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

void AProjectRPGQuest::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProjectRPGQuest, QuestName);
    DOREPLIFETIME(AProjectRPGQuest, Description);
    DOREPLIFETIME(AProjectRPGQuest, IsActive);
    DOREPLIFETIME(AProjectRPGQuest, Objectives);
}