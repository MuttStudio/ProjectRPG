#include "ProjectRPG.h"
#include "ProjectRPGPlaceable.h"

AProjectRPGPlaceable::AProjectRPGPlaceable(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    SetReplicates(true);
    bReplicateMovement = true;
}


