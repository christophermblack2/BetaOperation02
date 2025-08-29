#include "GE_FireballBurn.h"
#include "BOHealthAttributeSet.h"

UGE_FireballBurn::UGE_FireballBurn()
{
    DurationPolicy = EGameplayEffectDurationType::HasDuration;
    DurationMagnitude = FScalableFloat(4.f);
    Period = 0.5f;

    FGameplayModifierInfo Info;
    Info.Attribute = UBOHealthAttributeSet::GetHealthAttribute();
    Info.ModifierOp = EGameplayModOp::Additive;
    Info.ModifierMagnitude = FScalableFloat(-5.f);
    Modifiers.Add(Info);
}

