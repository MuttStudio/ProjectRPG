#pragma once

#include "ProjectRPGQuest.h"
#include "ProjectRPGQuestObjective.h"
#include "ProjectRPGItem.h"
#include "ProjectRPGWeapon.h"
#include "ProjectRPGConsumable.h"
#include "ProjectRPGSpell.h"
#include "ProjectRPGAlignmentSpell.h"
#include "ProjectRPGCharacter.generated.h"

UCLASS(config = Game)
class AProjectRPGCharacter : public ACharacter
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
        TArray<AProjectRPGItem*> ItemInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
        TArray<AProjectRPGItem*> ItemBar;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
        int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magic)
        int32 Alignment1Percentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magic)
        int32 Alignment2Percentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magic)
        TArray<AProjectRPGAlignmentSpell*> Alignment1Spells;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magic)
        TArray<AProjectRPGAlignmentSpell*> Alignment2Spells;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magic)
        TArray<AProjectRPGSpell*> BasicSpells;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Quest)
        TArray<AProjectRPGQuest*> Quests;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Quest)
        TArray<AProjectRPGQuest*> TurnedInQuests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
        TArray<AProjectRPGQuest*> QuestOffer;

    bool bDrawDebugViewTrace;

    void Tick(float DeltaSeconds) override;

    void PickUpItem(AProjectRPGItem* Item);
    void UseCurrentItem();
    void TryRemoveFromItemBar(AProjectRPGItem* Item);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Move Item", CompactNodeTitle = "MvItm", Keywords = "Move Item"), Category = Inventory)
        void MoveItem(int32 item1, int32 item2, int32 stackSize);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Inv", CompactNodeTitle = "GetInv", Keywords = "Get Player Inventory"), Category = Inventory)
        TArray<AProjectRPGItem*> GetCurrentInventory();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Item Bar", CompactNodeTitle = "GetItmBar", Keywords = "Get Player Item Bar"), Category = Inventory)
        TArray<AProjectRPGItem*> GetCurrentItemBar();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Move Item On Bar", CompactNodeTitle = "MvItmBar", Keywords = "Move Item Bar"), Category = Inventory)
        void MoveItemOnBar(int32 from, int32 to);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Add Item Bar", CompactNodeTitle = "AddItmBar", Keywords = "Add Item To Item Bar"), Category = Inventory)
        void AddItemToItemBar(int32 addIndex, int32 itemIndex);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Remove Item Bar", CompactNodeTitle = "RmvItmBar", Keywords = "Remove Item From Item Bar"), Category = Inventory)
        void RemoveItemBarItem(int32 index);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Remove Item Inventory", CompactNodeTitle = "RmvItmInv", Keywords = "Remove Item From Inventory"), Category = Inventory)
        void RemoveItemFromInventory(int32 index);

    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Item: Display Hover"))
        virtual void DisplayItemHover(AProjectRPGItem* newItem);

    // This is a hack because it seems like you can't pass in TArray<AProjectRPGQuest*> in this event
    UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Quest: Display Quests Offered"))
        virtual void DisplayQuestOffer();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        TSubobjectPtr<class UCameraComponent> FirstPersonCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        FVector GunOffset;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
        int32 InventoryBagSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
        int32 InventoryBags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
        int32 ItemBarSize;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectRPGProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        USoundBase* FireSound;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        UAnimMontage* AttackAnim;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        AProjectRPGWeapon* CurrentWeapon;

    UFUNCTION(reliable, server, WithValidation, BlueprintCallable, meta = (FriendlyName = "Pick Up Quest", CompactNodeTitle = "PckQst", Keywords = "Pick Up Quest"), Category = Quest)
        void PickUpQuest(AProjectRPGQuest* quest);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Abandon Quest", CompactNodeTitle = "AbdnQst", Keywords = "Abandon Quest"), Category = Quest)
        void AbandonQuest(AProjectRPGQuest* quest);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Turn In Quest", CompactNodeTitle = "TrnInQst", Keywords = "Turn In Quest"), Category = Quest)
        void TurnInQuest(AProjectRPGQuest* quest);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "DoneAttacking", CompactNodeTitle = "DnAtck", Keywords = "Done Attacking"), Category = Attack)
        void DoneAttacking();

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "StartAttacking", CompactNodeTitle = "StAtck", Keywords = "Start Attacking"), Category = Attack)
        void StartAttacking();

    UFUNCTION(reliable, server, WithValidation)
        void ServerDoneAttacking();

    UFUNCTION(reliable, server, WithValidation)
        void ServerStartAttacking();

    // TODO: Turn this into an enum to manage attack states
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = Attack)
        bool IsSwinging;

    float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

    //UFUNCTION(reliable, server, WithValidation)
    //    void ServerTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

protected:
    void ReplaceItemBarItem(AProjectRPGItem* Item1, AProjectRPGItem* Item2);

    bool TryInsertNonStackableItem(AProjectRPGItem* Item);

    bool TryInsertStackableItem(AProjectRPGItem* Item);

    void OnFirePress();

    UFUNCTION(reliable, server, WithValidation)
        void OnTryPickUp();

    void MoveForward(float Val);

    void MoveRight(float Val);

    void TurnAtRate(float Rate);

    void LookUpAtRate(float Rate);

    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};

