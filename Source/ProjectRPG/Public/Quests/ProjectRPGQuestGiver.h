#pragma once

#include "ProjectRPGQuest.h"
#include "GameFramework/Character.h"
#include "ProjectRPGQuestGiver.generated.h"

UCLASS()
class AProjectRPGQuestGiver : public ACharacter
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, Category = Behavior)
    class UBehaviorTree* QuestGiverBehavior;

    TArray<AProjectRPGQuest*> GetQuests();

    TArray<AProjectRPGQuest*> AvailableQuests;

    virtual void BeginPlay();
};
