#include "ProjectRPG.h"
#include "ProjectRPGItem.h"

AProjectRPGItem::AProjectRPGItem(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    EquipMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("EquipMesh"));
    EquipMesh->SetOnlyOwnerSee(false);
    SetRootComponent(EquipMesh);
    EquipMesh->bCastDynamicShadow = false;
    EquipMesh->CastShadow = false;

    DropMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("DropMesh"));
    DropMesh->SetOnlyOwnerSee(false);
    DropMesh->AttachParent = EquipMesh;
    DropMesh->bCastDynamicShadow = false;
    DropMesh->CastShadow = false;

    IsValid = false;
}

//void AProjectRPGItem::BeginPlay()
//{
//    Super::BeginPlay();
//    CurrentMesh ->SetSimulatePhysics(true);
//    CurrentMesh->WakeRigidBody();
//}

void AProjectRPGItem::PickedUp()
{
#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Picked up from Item");
#endif
    SetMeshType(none);
}

void AProjectRPGItem::DroppedAlt(FRotator rotation, FVector vector)
{
    vector = vector + rotation.RotateVector(FVector(100.0f, 30.0f, 10.0f));
    TSubclassOf<class AProjectRPGItem> ItemClass;
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

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = Instigator;

        AProjectRPGItem* const Item = world->SpawnActor<AProjectRPGItem>(vector, rotation, SpawnParams);
    }
}

void AProjectRPGItem::SetMeshType(MeshType type)
{
    switch (type)
    {
    case dropedMesh:
#ifdef UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hiding equip mesh");
#endif
        if (EquipMesh)
        {
            EquipMesh->DestroyComponent();
        }
        break;
    case equipedMesh:
#ifdef UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hiding drop mesh");
#endif
        if (DropMesh)
        {
            DropMesh->DestroyComponent();
        }
        break;
    case none:
#ifdef UE_BUILD_DEBUG
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
