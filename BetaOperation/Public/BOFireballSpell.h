#pragma once

#include "CoreMinimal.h"
#include "BOSpell.h"
#include "BOFireballSpell.generated.h"

class UGameplayEffect;

/** Fireball spell with fire element and burn DOT */
UCLASS()
class BETAOPERATION_API ABOFireballSpell : public ABOSpell
{
    GENERATED_BODY()
public:
    ABOFireballSpell();

protected:
    // Burn damage-over-time effect
    UPROPERTY(EditDefaultsOnly, Category="Spell")
    TSubclassOf<UGameplayEffect> BurnDotEffectClass;

    virtual void HandleImpact(const FHitResult& SweepResult) override;
};

