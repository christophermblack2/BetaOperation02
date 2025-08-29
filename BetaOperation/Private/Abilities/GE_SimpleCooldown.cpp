#include "Abilities/GE_SimpleCooldown.h"

UGE_SimpleCooldown::UGE_SimpleCooldown()
{
    // Minimal effect used purely for cooldown; duration is set on the outgoing spec by the ability
    DurationPolicy = EGameplayEffectDurationType::HasDuration;
}
