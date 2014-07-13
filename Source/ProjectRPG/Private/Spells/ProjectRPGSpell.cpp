#include "ProjectRPG.h"
#include "ProjectRPGSpell.h"


AProjectRPGSpell::AProjectRPGSpell(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    CastMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CastMesh"));
    CastMesh->SetOnlyOwnerSee(false);
    SetRootComponent(CastMesh);
    CastMesh->bCastDynamicShadow = false;
    CastMesh->CastShadow = false;
}
