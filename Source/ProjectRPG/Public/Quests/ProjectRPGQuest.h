#pragma once

#include "ProjectRPGQuestObjective.h"
#include "GameFramework/Actor.h"
#include "ProjectRPGQuest.generated.h"

UCLASS()
class AProjectRPGQuest : public AActor
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = Quest)
        FString QuestName;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = Quest)
        FString Description;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = Quest)
        bool IsActive;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = Quest)
        TArray<AProjectRPGQuestObjective*> Objectives;

    UFUNCTION(BlueprintCallable, Category = Quest)
        bool IsComplete();

    void AddOwner(AActor* owner);

    void Activate();
};
