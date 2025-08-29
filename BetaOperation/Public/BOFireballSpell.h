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

    // Cooldown checks callable from Blueprints
    UFUNCTION(BlueprintCallable, Category="Spell|Cooldown")
    bool CanCastNow() const;

    UFUNCTION(BlueprintCallable, Category="Spell|Cooldown")
    void StartCooldown();

protected:
    // Burn damage-over-time effect
    UPROPERTY(EditDefaultsOnly, Category="Spell")
    TSubclassOf<UGameplayEffect> BurnDotEffectClass;

    virtual void HandleImpact(const FHitResult& SweepResult) override;

private:
    // Next time (in seconds) when this spell can be cast again
    double NextCastTime = 0.0;
};

