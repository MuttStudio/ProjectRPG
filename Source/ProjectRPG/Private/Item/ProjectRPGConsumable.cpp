

#include "ProjectRPG.h"
#include "ProjectRPGConsumable.h"


AProjectRPGConsumable::AProjectRPGConsumable(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    // potions are always stackable
    isStackable = true;
}


