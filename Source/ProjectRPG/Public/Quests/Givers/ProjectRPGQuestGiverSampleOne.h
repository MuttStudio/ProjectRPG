#pragma once

#include "Quests/ProjectRPGQuestGiver.h"
#include "ProjectRPGQuestGiverSampleOne.generated.h"

UCLASS()
class AProjectRPGQuestGiverSampleOne : public AProjectRPGQuestGiver
{
	GENERATED_UCLASS_BODY()

public:
    virtual void BeginPlay();
};
