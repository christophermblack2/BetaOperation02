#include "BOFireballSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "GE_FireballBurn.h"

ABOFireballSpell::ABOFireballSpell()
{
    SpellName = TEXT("Fireball");
    Cooldown = 0.5f;
    DamageMagnitude = 25.f;
    Element = FGameplayTag::RequestGameplayTag(TEXT("Element.Fire"));
    AppliedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Damage.Type.Fire")));
    BurnDotEffectClass = UGE_FireballBurn::StaticClass();
}

void ABOFireballSpell::HandleImpact(const FHitResult& SweepResult)
{
    if (BurnDotEffectClass)
    {
        float Radius = 200.f;
        TArray<FOverlapResult> Overlaps;
        FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);
        GetWorld()->OverlapMultiByObjectType(Overlaps, SweepResult.ImpactPoint, FQuat::Identity,
            FCollisionObjectQueryParams(ECC_Pawn), SphereShape, Params);

        for (const FOverlapResult& Res : Overlaps)
        {
            AActor* Actor = Res.GetActor();
            if (!Actor || Actor == GetInstigator())
            {
                continue;
            }

            if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
            {
                FGameplayEffectContextHandle Context;
                if (SourceASC.IsValid())
                {
                    Context = SourceASC->MakeEffectContext();
                    Context.AddInstigator(GetInstigator(), GetInstigator());
                }
                else
                {
                    Context = TargetASC->MakeEffectContext();
                    Context.AddInstigator(GetInstigator(), GetInstigator());
                }

                FGameplayEffectSpecHandle DotSpec = TargetASC->MakeOutgoingSpec(BurnDotEffectClass, 1.f, Context);
                if (DotSpec.IsValid())
                {
                    TargetASC->ApplyGameplayEffectSpecToSelf(*DotSpec.Data.Get());
                }
            }
        }
    }

    Super::HandleImpact(SweepResult);
}

