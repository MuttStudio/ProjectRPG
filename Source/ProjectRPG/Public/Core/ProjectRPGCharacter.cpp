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
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectRPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    // set up gameplay key bindings
    check(InputComponent);

    InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    InputComponent->BindAction("Drop", IE_Pressed, this, &AProjectRPGCharacter::DropCurrentItem);

    InputComponent->BindAction("Fire", IE_Pressed, this, &AProjectRPGCharacter::OnFire);
    InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AProjectRPGCharacter::TouchStarted);

    InputComponent->BindAxis("MoveForward", this, &AProjectRPGCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AProjectRPGCharacter::MoveRight);


    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    InputComponent->BindAxis("TurnRate", this, &AProjectRPGCharacter::TurnAtRate);
    InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    InputComponent->BindAxis("LookUpRate", this, &AProjectRPGCharacter::LookUpAtRate);
}

void AProjectRPGCharacter::OnFire()
{
    // try and fire a projectile
    if (ProjectileClass != NULL)
    {
        const FRotator SpawnRotation = GetControlRotation();
        // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
        const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            // spawn the projectile at the muzzle
            World->SpawnActor<AProjectRPGProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
        }
    }

    // try and play the sound if specified
    if (FireSound != NULL)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // try and play a firing animation if specified
    if (FireAnimation != NULL)
    {
        // Get the animation object for the arms mesh
        UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
        if (AnimInstance != NULL)
        {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

void AProjectRPGCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    // only fire for first finger down
    if (FingerIndex == 0)
    {
        OnFire();
    }
}

void AProjectRPGCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // find out which way is forward
        const FRotator Rotation = GetControlRotation();
        FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0);

        // Get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AProjectRPGCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        // find out which way is right
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AProjectRPGCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectRPGCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectRPGCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FVector CamLoc;
    FRotator CamRot;

    Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
    const FVector StartTrace = CamLoc; // trace start is the camera location
    const FVector Direction = CamRot.Vector();
    const FVector EndTrace = StartTrace + Direction * 200;

    // Perform trace to retrieve hit info
    FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = true;

    FHitResult Hit(ForceInit);
    if (GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECC_WorldStatic, TraceParams))
    {
        AProjectRPGItem* NewItem = Cast<AProjectRPGItem>(Hit.GetActor()); // typecast to the item class to the hit actor
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

        if (NewItem) // if we hit an item with the trace
        {
            this->PickUpItem(NewItem); // pick it up
        }
    }
}

void AProjectRPGCharacter::PickUpItem(AProjectRPGItem* Item)
{
    if (Item)
    {
        AProjectRPGConsumable* t = GetWorld()->SpawnActor<AProjectRPGConsumable>();
        ItemInventory.Add(t); // add it to the array
        Item->PickedUp(); // hide mesh 
    }
}

TArray<AProjectRPGItem*> AProjectRPGCharacter::GetCurrentInventory()
{
    return ItemInventory;
}

void AProjectRPGCharacter::DropCurrentItem()
{
    AProjectRPGCharacter::OnFire();
    AProjectRPGConsumable* testItem = GetWorld()->SpawnActor<AProjectRPGConsumable>();
    ItemInventory.Add(testItem);

#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Testing");
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(ItemInventory.Num()));
#endif
}