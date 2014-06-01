#include "ProjectRPG.h"
#include "ProjectRPGItem.h"

AProjectRPGItem::AProjectRPGItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    //CurrentMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
    //RootComponent = CurrentMesh;
}

//void AProjectRPGItem::BeginPlay()
//{
//    Super::BeginPlay();
//    CurrentMesh ->SetSimulatePhysics(true);
//    CurrentMesh->WakeRigidBody();
//}

void AProjectRPGItem::PickedUp()
{
    //if (DropMesh)
    //{
    //    DropMesh->DestroyComponent(); // physical item has been picked up, destroy its visible component
    //}
}

void AProjectRPGItem::SetMeshType(MeshType type)
{
    switch (type)
    {
    case dropedMesh:
        break;
    case equipedMesh:
        break;
    default:
        break;
    }
}
