#pragma once

#include "GameFramework/Actor.h"
#include "ProjectRPGItem.generated.h"

/**
 * Represents an item that can be placed into the player's inventory
 */

UENUM(BlueprintType)
enum MeshType
{
    dropedMesh UMETA(DisplayName = "Drop Mesh"),
    equipedMesh UMETA(DisplayName = "Equip Mesh"),
    none UMETA(DisplayName = "Stashed")
};

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        int32 InventoryCols;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        int32 InventoryRows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        UMaterial* Icon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
        TSubobjectPtr<class UStaticMeshComponent> DropMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
        TSubobjectPtr<class UStaticMeshComponent> EquipMesh;

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Item: Used"))
        virtual void Used();

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Item: Dropped"))
        virtual void Dropped();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        bool isStackable;

    UFUNCTION(BlueprintCallable, Category = Item, meta = (FriendlyName = "Item: Change Mesh"))
        void SetMeshType(MeshType type);

    //virtual void BeginPlay() OVERRIDE;
    void PickedUp();
};
