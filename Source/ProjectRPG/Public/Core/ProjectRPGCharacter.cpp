// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ProjectRPG.h"
#include "ProjectRPGCharacter.h"
#include "ProjectRPGPlayerController.h"
#include "ProjectRPGProjectile.h"

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
    Mesh1P = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
    Mesh1P->AttachParent = FirstPersonCameraComponent;
    Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;

    // Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
    // derived blueprint asset named MyCharacter (to avoid direct content references in C++)

    InventoryBagSize = 20;
    InventoryBags = 1;
    ItemBarSize = 10;
    ItemBar.SetNum(ItemBarSize);
    ItemInventory.SetNum(InventoryBags * InventoryBagSize);

    BasicSpells.SetNum(20);
    Alignment1Spells.SetNum(12);
    Alignment2Spells.SetNum(12);
    Alignment1Percentage = 0;
    Alignment2Percentage = 0;
}

void AProjectRPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    check(InputComponent);

    InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    InputComponent->BindAction("PickUp", IE_Pressed, this, &AProjectRPGCharacter::OnTryPickUp);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AProjectRPGCharacter::OnFire);
    InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AProjectRPGCharacter::TouchStarted);

    InputComponent->BindAxis("MoveForward", this, &AProjectRPGCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AProjectRPGCharacter::MoveRight);

    InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    InputComponent->BindAxis("TurnRate", this, &AProjectRPGCharacter::TurnAtRate);
    InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    InputComponent->BindAxis("LookUpRate", this, &AProjectRPGCharacter::LookUpAtRate);
}

void AProjectRPGCharacter::OnFire()
{
    if (ProjectileClass != NULL)
    {
        const FRotator SpawnRotation = GetControlRotation();
        const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(SpawnRotation.Pitch));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(SpawnRotation.Roll));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(SpawnRotation.Yaw));

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(SpawnLocation.X));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(SpawnLocation.Y));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(SpawnLocation.Z));

        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            World->SpawnActor<AProjectRPGProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
        }
    }

    if (FireSound != NULL)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    if (FireAnimation != NULL)
    {
        UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
        if (AnimInstance != NULL)
        {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

void AProjectRPGCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    if (FingerIndex == 0)
    {
        OnFire();
    }
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

void AProjectRPGCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FVector CamLoc;
    FRotator CamRot;

    Controller->GetPlayerViewPoint(CamLoc, CamRot);
    const FVector StartTrace = CamLoc;
    const FVector Direction = CamRot.Vector();
    const FVector EndTrace = StartTrace + Direction * 200;

    FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
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

        DisplayItemHover(NewItem);
    }
    else
    {
        DisplayItemHover(NULL);
    }
}
void AProjectRPGCharacter::OnTryPickUp()
{
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
        return;

    AProjectRPGItem* first = ItemBar[item1];
    ItemBar[item1] = ItemBar[item2];
    ItemBar[item2] = first;
}

void AProjectRPGCharacter::MoveItem(int32 item1, int32 item2, int32 stackSize = 0)
{
    if (item1 == item2)
        return;

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
