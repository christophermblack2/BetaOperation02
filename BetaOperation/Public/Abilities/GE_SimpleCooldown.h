#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_SimpleCooldown.generated.h"

/** Minimal cooldown effect: HasDuration; actual duration is set on the outgoing spec by the ability. */
UCLASS()
class BETAOPERATION_API UGE_SimpleCooldown : public UGameplayEffect
{
    GENERATED_BODY()
public:
    UGE_SimpleCooldown();
};
