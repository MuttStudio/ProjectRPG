#include "ProjectRPG.h"
#include "ProjectRPGWeapon.h"

AProjectRPGWeapon::AProjectRPGWeapon(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
}

void AProjectRPGWeapon::DoneAttacking()
{
    IsAttacking = false;
    PlayersHit.Empty();
}
