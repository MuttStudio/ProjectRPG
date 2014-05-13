#pragma once
#include "ProjectRPGItem.h"
#include "ProjectRPGConsumable.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum SubitemType
{
    food UMETA(DisplayName = "Food"),
    drink UMETA(DisplayName = "Drink"),
    healing_potion UMETA(DisplayName = "Healing Potion"),
    posion_potion UMETA(DisplayName = "Posion Potion"),
    fire_potion UMETA(DisplayName = "Fire Potion")
};

UCLASS()
class AProjectRPGConsumable : public AProjectRPGItem
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        int32 Potentcy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        bool isThrowable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        TEnumAsByte<SubitemType> ConsumableType;

    //Used to decide if you can use this potion on a friendly target
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        bool isSharable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
        int32 maxStackSize;

};
