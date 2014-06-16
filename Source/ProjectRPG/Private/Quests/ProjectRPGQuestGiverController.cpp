#include "ProjectRPG.h"
#include "ProjectRPGQuestGiverController.h"
#include "ProjectRPGQuestGiver.h"
#include "ProjectRPGCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AProjectRPGQuestGiverController::AProjectRPGQuestGiverController(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    BlackboardComponent = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));

    BehaviorComponent = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComponent"));
}


void AProjectRPGQuestGiverController::Possess(class APawn *InPawn)
{
    Super::Possess(InPawn);
    AProjectRPGQuestGiver* QuestGiver = Cast<AProjectRPGQuestGiver>(InPawn);

    if (QuestGiver && QuestGiver->QuestGiverBehavior)
    {
        BlackboardComponent->InitializeBlackboard(QuestGiver->QuestGiverBehavior->BlackboardAsset);

        TargetKeyID = BlackboardComponent->GetKeyID("Target");
        TargetLocationID = BlackboardComponent->GetKeyID("Destination");

        BehaviorComponent->StartTree(QuestGiver->QuestGiverBehavior);
    }
}

bool AProjectRPGQuestGiverController::SearchForTarget()
{
    APawn* questGiver = GetPawn();
    if (!questGiver)
    {
        return false;
    }

    const FVector loc = questGiver->GetActorLocation();
    float minDist = 350;
    AProjectRPGCharacter* targetPawn = NULL;

    for (FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; it++)
    {
        AProjectRPGCharacter* testPawn = Cast<AProjectRPGCharacter>(*it);

        if (testPawn)
        {
            const float dist = FVector::Dist(testPawn->GetActorLocation(), loc);
            if (dist < minDist)
            {
                minDist = dist;
                targetPawn = testPawn;
                FRotator rot = FRotationMatrix::MakeFromX(targetPawn->GetActorLocation() - questGiver->GetActorLocation()).Rotator();
                FRotator newRot = questGiver->GetActorRotation();
                newRot.Yaw = rot.Yaw;
                questGiver->SetActorRotation(newRot);
            }

        }
    }

    if (targetPawn)
    {
        SetTarget(targetPawn);
        return true;
    }

    return false;
}

void AProjectRPGQuestGiverController::Wander()
{
    APawn* questGiver = GetPawn();
    if (!questGiver)
    {
        return;
    }

    int8 wanderTolerance = 100;

    FVector loc = questGiver->GetActorLocation();
    int32 xrand = FMath::RandRange(-500, 500);
    int32 yrand = FMath::RandRange(-500, 500);
    FVector newLoc = FVector(loc.X + xrand, loc.Y + yrand, loc.Z);

    if ((xrand > wanderTolerance*-1 && xrand < wanderTolerance) || (yrand > wanderTolerance*-1 && yrand < wanderTolerance))
    {
        BlackboardComponent->SetValueAsVector(TargetLocationID, newLoc);
    }
}

void AProjectRPGQuestGiverController::SetTarget(class APawn* InPawn)
{
    BlackboardComponent->SetValueAsObject(TargetKeyID, InPawn);
    BlackboardComponent->SetValueAsVector(TargetLocationID, InPawn->GetActorLocation());
}
