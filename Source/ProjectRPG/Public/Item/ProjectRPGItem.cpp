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
    DropMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);
    DropMesh->bCastDynamicShadow = false;
    DropMesh->CastShadow = false;

}

//void AProjectRPGItem::BeginPlay()
//{
//    Super::BeginPlay();
//    CurrentMesh ->SetSimulatePhysics(true);
//    CurrentMesh->WakeRigidBody();
//}

bool AProjectRPGItem::PickedUp()
{
#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Picked up from Item");
#endif
    SetMeshType(equipedMesh);
}

void AProjectRPGItem::SetMeshType(MeshType type)
{
    switch (type)
    {
    case dropedMesh:
#ifdef UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hiding drop equip mesh");
#endif
        EquipMesh->SetVisibility(false, false);
        DropMesh->SetVisibility(true, false);
        break;
    case equipedMesh:
#ifdef UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hiding drop drop mesh");
#endif
        EquipMesh->SetVisibility(true, false);
        DropMesh->SetVisibility(false, false);
        break;
    case none:
        EquipMesh->SetVisibility(false, false);
        DropMesh->SetVisibility(false, false);
        break;
    default:
        break;
    }
}
