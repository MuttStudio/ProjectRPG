#include "ProjectRPG.h"
#include "ProjectRPGItem.h"

AProjectRPGItem::AProjectRPGItem(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    EquipMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("EquipMesh"));
    EquipMesh->SetOnlyOwnerSee(false);
    EquipMesh->bCastDynamicShadow = true;
    EquipMesh->CastShadow = true;
    EquipMesh->BodyInstance.SetCollisionProfileName("Weapon");
    RootComponent = EquipMesh;

    DropMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("DropMesh"));
    DropMesh->SetOnlyOwnerSee(false);
    DropMesh->AttachParent = EquipMesh;
    DropMesh->bCastDynamicShadow = true;
    DropMesh->CastShadow = true;

    StackSize = 1;
}

void AProjectRPGItem::PickedUp(const ACharacter* newOwner)
{
#if UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Picked up from Item");
#endif
    this->ItemOwner = newOwner;
    SetMeshType(none);
    NotifyPickup();
}

void AProjectRPGItem::DroppedAlt(FRotator rotation, FVector vector)
{
    if (!IsValid(this))
        return;

    UClass* theClass = this->GetClass();
    UWorld* const world = GetWorld();
    if (world)
    {
        SetMeshType(dropedMesh);

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Dropping from item");

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(rotation.Pitch));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(rotation.Roll));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(rotation.Yaw));

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(vector.X));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(vector.Y));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(vector.Z));

        AProjectRPGItem* item = world->SpawnActor<AProjectRPGItem>(theClass, vector, rotation);
        item->InitParams(this);
    }
}

void AProjectRPGItem::InitParams(AProjectRPGItem* item)
{
    this->Icon = item->Icon;
    this->Description = item->Description;
    this->Lore = item->Lore;
    this->StackSize = item->StackSize;
    this->isStackable = item->isStackable;
    this->MaxStackSize = item->MaxStackSize;
    this->ObjectName = item->ObjectName;
}

void AProjectRPGItem::SetMeshType(MeshType type)
{
    switch (type)
    {
    case dropedMesh:
#if UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hiding equip mesh");
#endif
        if (EquipMesh)
        {
            EquipMesh->DestroyComponent();
        }
        break;
    case equipedMesh:
#if UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hiding drop mesh");
#endif
        if (DropMesh)
        {
            DropMesh->DestroyComponent();
        }
        break;
    case none:
#if UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hiding all meshes");
#endif
        if (EquipMesh)
        {
            EquipMesh->DestroyComponent();
        }
        if (DropMesh)
        {
            DropMesh->DestroyComponent();
        }
        break;
    default:
        break;
    }
}
