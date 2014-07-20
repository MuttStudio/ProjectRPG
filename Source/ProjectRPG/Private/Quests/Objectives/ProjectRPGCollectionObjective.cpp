#include "ProjectRPG.h"
#include "ProjectRPGCharacter.h"
#include "ProjectRPGItem.h"
#include "ProjectRPGCollectionObjective.h"

AProjectRPGCollectionObjective::AProjectRPGCollectionObjective(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
}

void AProjectRPGCollectionObjective::Tick(float deltaSeconds)
{
    Super::Tick(deltaSeconds);
    if (!IsActive || IsComplete)
    {
        return;
    }

    for (AActor* owner : Owners)
    {
        AProjectRPGCharacter* player = Cast<AProjectRPGCharacter>(owner);

        int32 currentCount = 0;
        for (AProjectRPGItem* item : player->ItemInventory)
        {
            if (item)
            {
                if (item->ObjectName == ObjectName)
                {
                    currentCount++;
                    if (currentCount >= Count)
                    {
                        SetComplete(player);
                        return;
                    }
                }
            }
        }
    }
}
