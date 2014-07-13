#pragma once

#include "ProjectRPGQuestObjective.h"
#include "GameFramework/Actor.h"
#include "ProjectRPGQuest.generated.h"

UCLASS()
class AProjectRPGQuest : public AActor
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = Quest)
        FString QuestName;

    UPROPERTY(BlueprintReadOnly, Category = Quest)
        FString Description;

    UPROPERTY(BlueprintReadWrite, Category = Quest)
        bool IsActive;

    UPROPERTY(BlueprintReadWrite, Category = Quest)
        TArray<AProjectRPGQuestObjective*> Objectives;

    UFUNCTION(BlueprintCallable, Category = Quest)
        bool IsComplete();

    void AddOwner(AActor* owner);

    void Activate();
};
