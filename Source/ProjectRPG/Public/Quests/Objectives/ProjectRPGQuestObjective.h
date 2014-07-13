#pragma once

#include "GameFramework/Actor.h"
#include "ProjectRPGQuestObjective.generated.h"

UCLASS()
class AProjectRPGQuestObjective : public AActor
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
        UMaterialInstance* MarkerMaterial;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
        FString ObjectiveName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
        FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
        bool isGroupCompleteable;

    void AddOwner(AActor* player);

    TArray<AActor*> Owners;

    AProjectRPGQuestObjective* NextInLine;

    bool GetIsActive();

    bool GetIsComplete();

    void SetActive();

    void SetComplete(AActor* completingPlayer);

protected:
    bool IsComplete;

    bool IsActive;
};
