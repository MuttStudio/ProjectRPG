#include "ProjectRPG.h"
#include "ProjectRPGQuestGiver.h"

AProjectRPGQuestGiver::AProjectRPGQuestGiver(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
}

TArray<AProjectRPGQuest*> AProjectRPGQuestGiver::GetQuests()
{
    return AvailableQuests;
}

void AProjectRPGQuestGiver::BeginPlay()
{
    CharacterMovement->MaxWalkSpeed = 150;
    CharacterMovement->RotationRate.Yaw = 1;
}
