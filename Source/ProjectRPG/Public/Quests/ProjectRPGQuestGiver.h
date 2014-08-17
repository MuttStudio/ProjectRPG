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

    //TArray<AProjectRPGQuest*> GetQuests();

    UPROPERTY(Replicated)
        TArray<AProjectRPGQuest*> AvailableQuests;

    virtual void BeginPlay();

    float Health;

    float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
};
