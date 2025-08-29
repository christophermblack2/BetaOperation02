#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_FireballBurn.generated.h"

/** Simple burn DOT effect: 5 dmg every 0.5s for 4s */
UCLASS()
class BETAOPERATION_API UGE_FireballBurn : public UGameplayEffect
{
    GENERATED_BODY()
public:
    UGE_FireballBurn();
};

