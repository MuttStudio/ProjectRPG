#include "ProjectRPG.h"
#include "ProjectRPGLocationObjective.h"
#include "ProjectRPGCollectionObjective.h"
#include "ProjectRPGQuestGiverController.h"
#include "ProjectRPGQuestGiverSampleTwo.h"

AProjectRPGQuestGiverSampleTwo::AProjectRPGQuestGiverSampleTwo(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    AIControllerClass = AProjectRPGQuestGiverController::StaticClass();
}

void AProjectRPGQuestGiverSampleTwo::BeginPlay()
{
    Super::BeginPlay();
    AProjectRPGQuest* q1 = GetWorld()->SpawnActor<AProjectRPGQuest>(AProjectRPGQuest::StaticClass());

    AProjectRPGLocationObjective* locObjective = GetWorld()->SpawnActor<AProjectRPGLocationObjective>(AProjectRPGLocationObjective::StaticClass());
    locObjective->TargetLocation = FVector(-230, 140, 158.5);
    locObjective->ValidRadius = 75;

    AProjectRPGCollectionObjective* colObjective = GetWorld()->SpawnActor<AProjectRPGCollectionObjective>(AProjectRPGCollectionObjective::StaticClass());
    colObjective->Count = 1;
    colObjective->ObjectName = "Axe";

    q1->QuestName = "They Are Comming!!";
    q1->Description = "Hi Traveler,\r\n I am in need of a weapon to defend against the evil that is coming.\r\nThere are a stash of axes around here somewhere. Find them and bring me one";

    q1->Objectives.Add(locObjective);
    q1->Objectives.Add(colObjective);

    AvailableQuests.Add(q1);
}
