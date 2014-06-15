// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ProjectRPGItem.h"
#include "ProjectRPGWeapon.h"
#include "ProjectRPGConsumable.h"
#include "ProjectRPGGenericItem.h"
#include "ProjectRPGCharacter.generated.h"

UCLASS(config = Game)
class AProjectRPGCharacter : public ACharacter
{
    GENERATED_UCLASS_BODY()

    /*Setup for the inventory*/
    UPROPERTY(EditAnywhere, Category = Inventory)
    TArray<AProjectRPGItem*> ItemInventory; // Our Inventory

    UPROPERTY(EditAnywhere, Category = Inventory)
        TArray<AProjectRPGItem*> ItemBar;

    bool bDrawDebugViewTrace;

    void Tick(float DeltaSeconds) OVERRIDE;

    void PickUpItem(AProjectRPGItem* Item);
    void UseCurrentItem();
    void TryRemoveFromItemBar(AProjectRPGItem* Item);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Inv", CompactNodeTitle = "GetInv", Keywords = "Get Player Inventory"), Category = Inventory)
        TArray<AProjectRPGItem*> GetCurrentInventory();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Item Bar", CompactNodeTitle = "GetItmBar", Keywords = "Get Player Item Bar"), Category = Inventory)
        TArray<AProjectRPGItem*> GetCurrentItemBar();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Move Item", CompactNodeTitle = "MvItm", Keywords = "Move Item"), Category = Inventory)
        void MoveItem(int32 item1, int32 item2);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Move Item On Bar", CompactNodeTitle = "MvItmBar", Keywords = "Move Item Bar"), Category = Inventory)
        void MoveItemOnBar(int32 from, int32 to);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Add Item Bar", CompactNodeTitle = "AddItmBar", Keywords = "Add Item To Item Bar"), Category = Inventory)
        void AddItemToItemBar(int32 addIndex, int32 itemIndex);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Remove Item Bar", CompactNodeTitle = "RmvItmBar", Keywords = "Remove Item From Item Bar"), Category = Inventory)
        void RemoveItemBarItem(int32 index);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Remove Item Inventory", CompactNodeTitle = "RmvItmInv", Keywords = "Remove Item From Inventory"), Category = Inventory)
        void RemoveItemFromInventory(int32 index);

    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
        TSubobjectPtr<class USkeletalMeshComponent> Mesh1P;

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        TSubobjectPtr<class UCameraComponent> FirstPersonCameraComponent;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;

    /** Gun muzzle's offset from the characters location */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        FVector GunOffset;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
        int32 InventoryBagSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
        int32 InventoryBags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
        int32 ItemBarSize;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectRPGProjectile> ProjectileClass;

    /** Sound to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        USoundBase* FireSound;

    /** AnimMontage to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        UAnimMontage* FireAnimation;

protected:

    /** Handler for a touch input beginning. */
    void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

    bool TryInsertNonStackableItem(AProjectRPGItem* Item);

    bool TryInsertStackableItem(AProjectRPGItem* Item);

    /** Fires a projectile. */
    void OnFire();

    /** Handles moving forward/backward */
    void MoveForward(float Val);

    /** Handles stafing movement, left and right */
    void MoveRight(float Val);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
    // End of APawn interface
};

