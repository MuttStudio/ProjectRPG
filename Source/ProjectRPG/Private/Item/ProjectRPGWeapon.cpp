#include "ProjectRPG.h"
#include "ProjectRPGWeapon.h"
#include "UnrealNetwork.h"

AProjectRPGWeapon::AProjectRPGWeapon(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
}

void AProjectRPGWeapon::DoneAttacking()
{
    IsAttacking = false;
    PlayersHit.Empty();
}

void AProjectRPGWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProjectRPGWeapon, IsAttacking);
}
