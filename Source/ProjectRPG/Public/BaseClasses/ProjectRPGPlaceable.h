#pragma once

#include "GameFramework/Actor.h"
#include "ProjectRPGPlaceable.generated.h"

/**
 *
 */
UCLASS()
class AProjectRPGPlaceable : public AActor
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placeable)
        UTexture* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placeable)
        FString ObjectName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placeable)
        FString Lore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placeable)
        FString Description;
};
