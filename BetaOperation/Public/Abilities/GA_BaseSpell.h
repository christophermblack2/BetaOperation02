#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BaseSpell.generated.h"

class ABOSpell;
class UGameplayEffect;

/** Base GameplayAbility that commits cost/cooldown and spawns a BOSpell actor */
UCLASS(Abstract)
class BETAOPERATION_API UGA_BaseSpell : public UGameplayAbility
{
    GENERATED_BODY()
public:
    UGA_BaseSpell();

protected:
    // Spell actor to spawn
    UPROPERTY(EditDefaultsOnly, Category="Spell")
    TSubclassOf<ABOSpell> SpellClass;

    // Optional damage override passed to InitializeSpell
    UPROPERTY(EditDefaultsOnly, Category="Spell")
    float DamageOverride = -1.f;

    // Cooldown effect class (applied on CommitAbility)
    UPROPERTY(EditDefaultsOnly, Category="Ability|Cooldown")
    TSubclassOf<UGameplayEffect> CooldownEffectClass;

    // Cooldown duration in seconds (applied to the generated spec)
    UPROPERTY(EditDefaultsOnly, Category="Ability|Cooldown", meta=(ClampMin="0.0"))
    float CooldownDuration = 0.f;

    // UGameplayAbility interface
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
                               const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo) const override;

private:
};
