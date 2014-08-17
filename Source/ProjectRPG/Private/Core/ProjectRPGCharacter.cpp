// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ProjectRPG.h"
#include "ProjectRPGCharacter.h"
#include "ProjectRPGPlayerController.h"
#include "ProjectRPGProjectile.h"
#include "ProjectRPGQuestGiver.h"
#include "UnrealNetwork.h"

/////////////////////////////////////////////////////////////////////////
// AProjectRPGCharacter

AProjectRPGCharacter::AProjectRPGCharacter(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
#if UE_BUILD_DEBUG
    bDrawDebugViewTrace = true;
#endif

    // Set size for collision capsule
    CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
    CapsuleComponent->BodyInstance.SetCollisionProfileName("Capsule");

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Create a CameraComponent	
    FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->AttachParent = CapsuleComponent;
    FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera

    // Default offset from the character location for projectiles to spawn
    GunOffset = FVector(100.0f, 30.0f, 10.0f);

    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    //Mesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
    Mesh->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
    Mesh->AttachParent = FirstPersonCameraComponent;
    Mesh->RelativeLocation = FVector(0.f, 0.f, -150.f);
    Mesh->bCastDynamicShadow = true;
    Mesh->CastShadow = true;
    Mesh->BodyInstance.SetCollisionProfileName("CharacterMesh");
    //Mesh1P->BodyInstance.collision

    // Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
    // derived blueprint asset named MyCharacter (to avoid direct content references in C++)

    InventoryBagSize = 20;
    InventoryBags = 1;
    ItemBarSize = 4;
    ItemBar.SetNum(ItemBarSize);
    ItemInventory.SetNum(InventoryBags * InventoryBagSize);

    BasicSpells.SetNum(3);
    Alignment1Spells.SetNum(4);
    Alignment2Spells.SetNum(4);
    Alignment1Percentage = 0;
    Alignment2Percentage = 0;
    Health = 100;

    SetReplicates(true);
}

void AProjectRPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    check(InputComponent);

    InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    InputComponent->BindAction("PickUp", IE_Pressed, this, &AProjectRPGCharacter::OnTryPickUp);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AProjectRPGCharacter::OnFirePress);

    InputComponent->BindAxis("MoveForward", this, &AProjectRPGCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AProjectRPGCharacter::MoveRight);

    InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    InputComponent->BindAxis("TurnRate", this, &AProjectRPGCharacter::TurnAtRate);
    InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    InputComponent->BindAxis("LookUpRate", this, &AProjectRPGCharacter::LookUpAtRate);
}


float AProjectRPGCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
    //if (Role < ROLE_Authority)
    //{
    //    ServerTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    //}

    FHitResult hit;
    FVector impulse;
    DamageEvent.GetBestHitInfo(this, DamageCauser, hit, impulse);
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "getting hit");
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, hit.BoneName.ToString());
    Health -= 1;
    return Health;
}

//bool AProjectRPGCharacter::ServerTakeDamage_Validate(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
//{
//    return true;
//}
//
//void AProjectRPGCharacter::ServerTakeDamage_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
//{
//    this->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//}

void AProjectRPGCharacter::OnFirePress()
{
    StartAttacking();
}

void AProjectRPGCharacter::DoneAttacking()
{
    if (Role < ROLE_Authority)
    {
        ServerDoneAttacking();
        return;
    }

    IsSwinging = false;
    if (CurrentWeapon)
    {
        CurrentWeapon->DoneAttacking();
    }
}

bool AProjectRPGCharacter::ServerDoneAttacking_Validate()
{
    return true;
}

void AProjectRPGCharacter::ServerDoneAttacking_Implementation()
{
    DoneAttacking();
}

void AProjectRPGCharacter::StartAttacking()
{
    if (Role < ROLE_Authority)
    {
        ServerStartAttacking();
        return;
    }

    // To make sure we don't get stuck in a state that we don't want lets check if we are punching first
    // The anim montage normally makes sure we don't have this problem but just in case... 
    if (IsSwinging)
    {
        DoneAttacking();
        return;
    }

    // TODO: Later we will put the logic that decides what type of swing is used in here
    IsSwinging = true;
    if (CurrentWeapon)
    {
        CurrentWeapon->IsAttacking = true;
    }
}

bool AProjectRPGCharacter::ServerStartAttacking_Validate()
{
    return true;
}

void AProjectRPGCharacter::ServerStartAttacking_Implementation()
{
    StartAttacking();
}

void AProjectRPGCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        const FRotator Rotation = GetControlRotation();
        FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        AddMovementInput(Direction, Value);
    }
}

void AProjectRPGCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(Direction, Value);
    }
}

void AProjectRPGCharacter::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectRPGCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectRPGCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProjectRPGCharacter, IsSwinging);
    DOREPLIFETIME(AProjectRPGCharacter, Health);
    DOREPLIFETIME(AProjectRPGCharacter, AttackAnim);
    DOREPLIFETIME(AProjectRPGCharacter, CurrentWeapon);
    DOREPLIFETIME(AProjectRPGCharacter, Quests);
    DOREPLIFETIME(AProjectRPGCharacter, TurnedInQuests);
}

void AProjectRPGCharacter::Tick(float DeltaSeconds)
{
    if (!HasAuthority())
    {
        return;
    }

    Super::Tick(DeltaSeconds);

    FVector CamLoc;
    FRotator CamRot;

    Controller->GetPlayerViewPoint(CamLoc, CamRot);
    const FVector StartTrace = CamLoc;
    const FVector Direction = CamRot.Vector();
    const FVector EndTrace = StartTrace + Direction * 200;

    FCollisionQueryParams TraceParams(FName(TEXT("InfoTrace")), true, this);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = true;
    float radius = 20;
    FHitResult Hit(ForceInit);
    if (GetWorld()->SweepSingle(Hit, StartTrace, EndTrace, FQuat(1.0, 1.0, 1.0, 1.0), ECC_WorldStatic, FCollisionShape::MakeSphere(radius), TraceParams, FCollisionResponseParams()))
    {
        AProjectRPGItem* NewItem = Cast<AProjectRPGItem>(Hit.GetActor());
        if (bDrawDebugViewTrace)
        {
            DrawDebugLine(
                GetWorld(),
                StartTrace,
                EndTrace,
                FColor(255, 0, 0),
                false,
                3,
                0,
                1
                );
        }

        if (NewItem)
        {
            DisplayItemHover(NewItem);
        }
    }
    else
    {
        DisplayItemHover(NULL);
    }
}

bool AProjectRPGCharacter::OnTryPickUp_Validate()
{
    return true;
}

void AProjectRPGCharacter::OnTryPickUp_Implementation()
{
    if (!HasAuthority())
    {
        return;
    }

    AProjectRPGPlayerController* cont = Cast<AProjectRPGPlayerController>(Controller);
    if (cont->inQuestGiveMode)
    {
        cont->ToggleQuestGiveMode();
        return;
    }

    FVector CamLoc;
    FRotator CamRot;

    Controller->GetPlayerViewPoint(CamLoc, CamRot);
    const FVector StartTrace = CamLoc + CamRot.Vector() * 100;
    const FVector Direction = CamRot.Vector();
    const FVector EndTrace = StartTrace + Direction * 200;


    FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = true;
    TraceParams.bFindInitialOverlaps = true;
    float Radius = 150;

    // Pickups
    TArray<FHitResult> Hits;
    TArray<AProjectRPGItem*> NewItems = TArray<AProjectRPGItem*>();
    if (GetWorld()->SweepMulti(Hits, StartTrace, EndTrace, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(Radius), TraceParams))
    {
        for (FHitResult& hit : Hits)
        {
            AProjectRPGItem* NewItem = Cast<AProjectRPGItem>(hit.GetActor());
            if (bDrawDebugViewTrace)
            {
                DrawDebugSphere(
                    GetWorld(),
                    StartTrace,
                    Radius,
                    32,
                    FColor(255, 0, 0),
                    false,
                    10.0f
                    );
            }

            if (NewItem)
            {
                NewItems.AddUnique(NewItem);
            }
        }
    }

    for (AProjectRPGItem* item : NewItems)
    {
        PickUpItem(item);
    }

    // Interactions
    float radius = 5;
    FHitResult Hit(ForceInit);
    if (GetWorld()->SweepSingle(Hit, StartTrace, EndTrace, FQuat(1.0, 1.0, 1.0, 1.0), ECC_WorldStatic, FCollisionShape::MakeSphere(radius), TraceParams, FCollisionResponseParams()))
    {
        if (bDrawDebugViewTrace)
        {
            DrawDebugLine(
                GetWorld(),
                StartTrace,
                EndTrace,
                FColor(255, 0, 0),
                false,
                3,
                0,
                1
                );
        }

        AProjectRPGQuestGiver* questGiver = Cast<AProjectRPGQuestGiver>(Hit.GetActor());
        if (questGiver)
        {
            QuestOffer = questGiver->AvailableQuests;

            for (int i = 0; i < QuestOffer.Num(); i++)
            {
                if (TurnedInQuests.Contains(QuestOffer[i]))
                {
                    QuestOffer.RemoveAt(i);
                    i--;
                }
                else if (QuestOffer[i]->IsComplete())
                {
                    continue;
                }
                else if (Quests.Contains(QuestOffer[i]))
                {
                    QuestOffer.RemoveAt(i);
                    i--;
                }
            }

            DisplayQuestOffer();
            cont->ToggleQuestGiveMode();
        }
    }
}

void AProjectRPGCharacter::PickUpItem(AProjectRPGItem* Item)
{
#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Picking up from Character");
#endif
    if (Item)
    {
        if ((Item->isStackable && TryInsertStackableItem(Item)) || TryInsertNonStackableItem(Item))
        {
            Item->PickedUp();
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Inventory full");
        }
    }
#if UE_BUILD_DEBUG
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ERROR: Could not pick up item");
    }

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Picked up from Character");
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(ItemInventory.Num()));
#endif

}

bool AProjectRPGCharacter::TryInsertStackableItem(AProjectRPGItem* Item)
{
    if (Item)
    {
        if (Item->isStackable)
        {
            for (int i = 0; i < ItemInventory.Num(); i++)
            {
                if (ItemInventory[i] && ItemInventory[i]->ObjectName == Item->ObjectName)
                {
                    if (ItemInventory[i]->StackSize + Item->StackSize < ItemInventory[i]->MaxStackSize)
                    {
                        ItemInventory[i]->StackSize += Item->StackSize;
                        return true;
                    }
                    else
                    {
                        Item->StackSize -= ItemInventory[i]->MaxStackSize - ItemInventory[i]->StackSize;
                        ItemInventory[i]->StackSize = ItemInventory[i]->MaxStackSize;
                        if (Item->StackSize == 0)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool AProjectRPGCharacter::TryInsertNonStackableItem(AProjectRPGItem* Item)
{
    for (int i = 0; i < ItemInventory.Num(); i++)
    {
        if (!ItemInventory[i])
        {
            ItemInventory[i] = Item;
            return true;
        }
    }

    return false;
}
TArray<AProjectRPGItem*> AProjectRPGCharacter::GetCurrentInventory()
{
    return ItemInventory;
}

TArray<AProjectRPGItem*> AProjectRPGCharacter::GetCurrentItemBar()
{
    return ItemBar;
}

void AProjectRPGCharacter::RemoveItemBarItem(int32 index)
{
    ItemBar.RemoveAt(index);
    ItemBar.InsertZeroed(index);
}

void AProjectRPGCharacter::TryRemoveFromItemBar(AProjectRPGItem* Item)
{
    for (int i = 0; i < ItemBar.Num(); i++)
    {
        if (ItemBar[i] == Item)
        {
            RemoveItemBarItem(i);
        }
    }
}

void AProjectRPGCharacter::ReplaceItemBarItem(AProjectRPGItem* Item1, AProjectRPGItem* Item2)
{
    int32 one = -1;
    int32 two = -1;
    for (int i = 0; i < ItemBar.Num(); i++)
    {
        if (ItemBar[i] == Item1)
        {
            one = i;
        }
        else if (ItemBar[i] == Item2)
        {
            two = i;
        }
    }

    if (one >= 0 && two >= 0)
    {
        MoveItemOnBar(two, one);
        TryRemoveFromItemBar(Item2);
    }

    for (int i = 0; i < ItemBar.Num(); i++)
    {
        if (ItemBar[i] == Item1)
        {
            ItemBar[i] = Item2;
        }
    }
}

void AProjectRPGCharacter::RemoveItemFromInventory(int32 index)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Dropping");
    const FRotator SpawnRotation = GetControlRotation();
    const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
    ItemInventory[index]->DroppedAlt(SpawnRotation, SpawnLocation);
    TryRemoveFromItemBar(ItemInventory[index]);
    ItemInventory.RemoveAt(index);
    ItemInventory.InsertZeroed(index);
}

void AProjectRPGCharacter::AddItemToItemBar(int32 from, int32 to)
{
    ItemBar[to] = ItemInventory[from];
}

void AProjectRPGCharacter::MoveItemOnBar(int32 item1, int32 item2)
{
    if (item1 == item2)
    {
        return;
    }

    AProjectRPGItem* first = ItemBar[item1];
    ItemBar[item1] = ItemBar[item2];
    ItemBar[item2] = first;
}

bool AProjectRPGCharacter::PickUpQuest_Validate(AProjectRPGQuest* quest)
{
    return true;
}

void AProjectRPGCharacter::PickUpQuest_Implementation(AProjectRPGQuest* quest)
{
    if (!HasAuthority())
    {
        return;
    }

    for (FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; it++)
    {
        AProjectRPGCharacter* player = Cast<AProjectRPGCharacter>(*it);
        if (player)
        {
            if (player->Quests.Contains(quest))
            {
               continue;
            }

            player->Quests.Add(quest);
            quest->Activate();
            quest->AddOwner(player);
        }
    }
}

void AProjectRPGCharacter::AbandonQuest(AProjectRPGQuest* quest)
{
    for (FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; it++)
    {
        AProjectRPGCharacter* player = Cast<AProjectRPGCharacter>(*it);
        if (player)
        {
            player->Quests.Remove(quest);
        }
    }
}

void AProjectRPGCharacter::TurnInQuest(AProjectRPGQuest* quest)
{
    if (!quest)
    {
        return;
    }

    if (!quest->IsComplete())
    {
        return;
    }

    for (FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; it++)
    {
        AProjectRPGCharacter* player = Cast<AProjectRPGCharacter>(*it);
        if (player)
        {
            player->Quests.Remove(quest);
            player->QuestOffer.Remove(quest);
            player->TurnedInQuests.Add(quest);
            player->DisplayQuestOffer();
        }
    }
}

void AProjectRPGCharacter::MoveItem(int32 item1, int32 item2, int32 stackSize = 0)
{
    if (item1 == item2)
    {
        return;
    }

    if (stackSize == 0)
    {
        if (IsValid(ItemInventory[item1]) && ItemInventory[item1]->isStackable && IsValid(ItemInventory[item2]) && ItemInventory[item2]->isStackable && ItemInventory[item1]->ObjectName == ItemInventory[item2]->ObjectName)
        {
            if (ItemInventory[item1]->StackSize + ItemInventory[item2]->StackSize <= ItemInventory[item2]->MaxStackSize)
            {
                ItemInventory[item2]->StackSize += ItemInventory[item1]->StackSize;
                ReplaceItemBarItem(ItemInventory[item1], ItemInventory[item2]);
                ItemInventory.RemoveAt(item1);
                ItemInventory.InsertZeroed(item1);
            }
            else if (ItemInventory[item2]->StackSize == ItemInventory[item2]->MaxStackSize && ItemInventory[item1]->StackSize < ItemInventory[item1]->MaxStackSize)
            {
                AProjectRPGItem* first = ItemInventory[item1];
                ItemInventory[item1] = ItemInventory[item2];
                ItemInventory[item2] = first;
            }
            else
            {
                ItemInventory[item1]->StackSize -= ItemInventory[item2]->MaxStackSize - ItemInventory[item2]->StackSize;
                ItemInventory[item2]->StackSize = ItemInventory[item2]->MaxStackSize;
            }
        }
        else
        {
            AProjectRPGItem* first = ItemInventory[item1];
            ItemInventory[item1] = ItemInventory[item2];
            ItemInventory[item2] = first;
        }
    }
    else if (IsValid(ItemInventory[item1]) && ItemInventory[item1]->isStackable && IsValid(ItemInventory[item2]) && ItemInventory[item1]->ObjectName == ItemInventory[item2]->ObjectName && ItemInventory[item2]->StackSize + stackSize <= ItemInventory[item2]->MaxStackSize)
    {
        ItemInventory[item2]->StackSize += stackSize;
        ItemInventory[item1]->StackSize -= stackSize;
        if (ItemInventory[item1]->StackSize < 1)
        {
            TryRemoveFromItemBar(ItemInventory[item1]);
            ItemInventory.RemoveAt(item1);
            ItemInventory.InsertZeroed(item1);;
        }
    }
    else if (IsValid(ItemInventory[item1]) && ItemInventory[item1]->isStackable && !IsValid(ItemInventory[item2]) && stackSize <= ItemInventory[item1]->MaxStackSize)
    {
        const FRotator SpawnRotation = GetControlRotation();
        const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
        AProjectRPGItem* newItem = GetWorld()->SpawnActor<AProjectRPGItem>(ItemInventory[item1]->GetClass(), SpawnLocation, SpawnRotation);

        newItem->InitParams(ItemInventory[item1]);
        newItem->SetMeshType(none);
        newItem->StackSize = stackSize;
        ItemInventory[item1]->StackSize -= stackSize;
        if (ItemInventory[item1]->StackSize < 1)
        {
            TryRemoveFromItemBar(ItemInventory[item1]);
            ItemInventory.RemoveAt(item1);
            ItemInventory.InsertZeroed(item1);;
        }
        ItemInventory[item2] = newItem;
    }
}
