#pragma once

#include "GameFramework/Actor.h"
#include "ProjectRPGItem.generated.h"

/**
 * Represents an item that can be placed into the player's inventory
 */
UCLASS()
class AProjectRPGItem : public AActor
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        FString Lore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        FString Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        int32 Value;

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Item: Used"))
        virtual void Used();

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Item: Dropped"))
        virtual void Dropped();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
        TSubobjectPtr<class UStaticMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
    bool isStackable;

    virtual void BeginPlay() OVERRIDE;
    void PickedUp();
};
