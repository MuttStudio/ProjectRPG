#pragma once

#include "Quests/Objectives/ProjectRPGQuestObjective.h"
#include "ProjectRPGCollectionObjective.generated.h"

UCLASS()
class AProjectRPGCollectionObjective : public AProjectRPGQuestObjective
{
    GENERATED_UCLASS_BODY()

    void Tick(float DeltaSeconds) override;

    FString ObjectName;

    int32 Count;
};
