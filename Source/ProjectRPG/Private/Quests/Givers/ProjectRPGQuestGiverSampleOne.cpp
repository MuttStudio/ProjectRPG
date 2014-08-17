#include "ProjectRPG.h"
#include "ProjectRPGLocationObjective.h"
#include "ProjectRPGCollectionObjective.h"
#include "ProjectRPGQuestGiverController.h"
#include "ProjectRPGQuestGiverSampleOne.h"

AProjectRPGQuestGiverSampleOne::AProjectRPGQuestGiverSampleOne(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    AIControllerClass = AProjectRPGQuestGiverController::StaticClass();
}

void AProjectRPGQuestGiverSampleOne::BeginPlay()
{
    //if (!HasAuthority())
    //{
    //    return;
    //}

    Super::BeginPlay();
    AProjectRPGQuest* q1 = GetWorld()->SpawnActor<AProjectRPGQuest>(AProjectRPGQuest::StaticClass());
    AProjectRPGQuest* q2 = GetWorld()->SpawnActor<AProjectRPGQuest>(AProjectRPGQuest::StaticClass());

    AProjectRPGLocationObjective* locObjective = GetWorld()->SpawnActor<AProjectRPGLocationObjective>(AProjectRPGLocationObjective::StaticClass());
    locObjective->TargetLocation = FVector(-300, -1055, 190);
    locObjective->ValidRadius = 150;

    AProjectRPGCollectionObjective* colObjective = GetWorld()->SpawnActor<AProjectRPGCollectionObjective>(AProjectRPGCollectionObjective::StaticClass());
    colObjective->Count = 1;
    colObjective->ObjectName = "Axe";

    q1->QuestName = "Pickup an Axe";
    q1->Description = "Find an Axe";
    q1->Objectives.Add(colObjective);

    q2->QuestName = "Find The Hammer";
    q2->Description = "Walk to the hammer";
    q2->Objectives.Add(locObjective);

    AvailableQuests.Add(q1);
    AvailableQuests.Add(q2);
}