#pragma once

//#include "ProjectRPGQuest.h"
#include "ProjectRPGCharacter.h"
#include "AIController.h"
#include "ProjectRPGQuestGiverController.generated.h"

UCLASS()
class AProjectRPGQuestGiverController : public AAIController
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(transient)
        TSubobjectPtr<class UBlackboardComponent> BlackboardComponent;

    UPROPERTY(transient)
        TSubobjectPtr<class UBehaviorTreeComponent> BehaviorComponent;

    virtual void Possess(class APawn* InPawn);

    void SetTarget(class APawn* InPawn);

    UFUNCTION(BlueprintCallable, Category = Behavior)
        bool SearchForTarget();

    UFUNCTION(BlueprintCallable, Category = Behavior)
        void Wander();

protected:
    uint8 TargetKeyID;

    uint8 TargetLocationID;
};
