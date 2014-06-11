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
}

void AProjectRPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    check(InputComponent);

    InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
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

    FHitResult Hit(ForceInit);
    if (GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECC_WorldStatic, TraceParams))
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
            this->PickUpItem(NewItem);
        }
    }
}

void AProjectRPGCharacter::PickUpItem(AProjectRPGItem* Item)
{
#ifdef UE_BUILD_DEBUG
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
#ifdef UE_BUILD_DEBUG
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
                if (ItemInventory[i]->StackSize < ItemInventory[i]->MaxStackSize)
                {
                    ItemInventory[i]->StackSize++;
                    return true;
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
            Item->IsValid = true;
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

void AProjectRPGCharacter::RemoveItemFromInventory(int32 index)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Dropping");
    const FRotator SpawnRotation = GetControlRotation();
    const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
    ItemInventory[index]->DroppedAlt(SpawnRotation, SpawnLocation);
    //ItemInventory[index]->Dropped();
    ItemInventory.RemoveAt(index);
    ItemInventory.InsertZeroed(index);
}

void AProjectRPGCharacter::AddItemToItemBar(int32 from, int32 to)
{
    ItemBar[to] = ItemInventory[from];
}


void AProjectRPGCharacter::MoveItemOnBar(int32 item1, int32 item2)
{
    if (item1 <= ItemBar.Num())
    {
        AProjectRPGItem* first = ItemBar[item1];

        if (item2 <= ItemBar.Num())
        {
            ItemBar[item1] = ItemBar[item2];
            ItemBar[item2] = first;
        }
        else
        {
            ItemBar[item1] = GetWorld()->SpawnActor<AProjectRPGConsumable>();
            ItemBar.Insert(first, item2);
        }
    }
    else if (item2 <= ItemBar.Num())
    {
        AProjectRPGItem* first = ItemBar[item2];

        if (item1 <= ItemBar.Num())
        {
            ItemBar[item2] = ItemBar[item1];
            ItemBar[item1] = first;
        }
        else
        {
            ItemBar[item2] = GetWorld()->SpawnActor<AProjectRPGItem>();
            ItemBar.Insert(first, item1);
        }
    }
}

void AProjectRPGCharacter::MoveItem(int32 item1, int32 item2)
{
    if (item1 <= ItemInventory.Num())
    {
        AProjectRPGItem* first = ItemInventory[item1];

        if (item2 <= ItemInventory.Num())
        {
            ItemInventory[item1] = ItemInventory[item2];
            ItemInventory[item2] = first;
        }
        else
        {
            ItemInventory[item1] = GetWorld()->SpawnActor<AProjectRPGConsumable>();
            ItemInventory.Insert(first, item2);
        }
    }
    else if (item2 <= ItemInventory.Num())
    {
        AProjectRPGItem* first = ItemInventory[item2];

        if (item1 <= ItemInventory.Num())
        {
            ItemInventory[item2] = ItemInventory[item1];
            ItemInventory[item1] = first;
        }
        else
        {
            ItemInventory[item2] = GetWorld()->SpawnActor<AProjectRPGItem>();
            ItemInventory.Insert(first, item1);
        }
    }
}