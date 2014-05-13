#include "ProjectRPG.h"
#include "ProjectRPGItem.h"

AProjectRPGItem::AProjectRPGItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
    RootComponent = Mesh;
}

void AProjectRPGItem::BeginPlay()
{
    Super::BeginPlay();
    Mesh->SetSimulatePhysics(true);
    Mesh->WakeRigidBody();
}

void AProjectRPGItem::PickedUp()
{
    if (Mesh)
    {
        Mesh->DestroyComponent(); // physical item has been picked up, destroy its visible component
    }
}
