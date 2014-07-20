#pragma once

#include "Quests/Objectives/ProjectRPGQuestObjective.h"
#include "ProjectRPGLocationObjective.generated.h"

UCLASS()
class AProjectRPGLocationObjective : public AProjectRPGQuestObjective
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
        FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
        int32 ValidRadius;

    void Tick(float DeltaSeconds) override;
};
