#include "Abilities/GA_BaseSpell.h"
#include "BOSpell.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UGA_BaseSpell::UGA_BaseSpell()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGA_BaseSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                    const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEnd*/false, /*bWasCancelled*/true);
        return;
    }

    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid() || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    if (!SpellClass)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    AActor* Avatar = ActorInfo->AvatarActor.Get();
    UWorld* World = Avatar ? Avatar->GetWorld() : nullptr;
    if (!World)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Avatar;
    SpawnParams.Instigator = Cast<APawn>(Avatar);

    const FVector SpawnLoc = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * 100.f;
    const FRotator SpawnRot = Avatar->GetActorRotation();

    if (ABOSpell* Spell = World->SpawnActor<ABOSpell>(SpellClass, SpawnLoc, SpawnRot, SpawnParams))
    {
        if (DamageOverride >= 0.f)
        {
            Spell->InitializeSpell(ActorInfo->AbilitySystemComponent.Get(), DamageOverride);
        }
        else
        {
            // Use ABOSpell's default damage via default parameter
            Spell->InitializeSpell(ActorInfo->AbilitySystemComponent.Get());
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/false);
}

void UGA_BaseSpell::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo) const
{
    if (CooldownEffectClass && CooldownDuration > 0.f && ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
    {
        FGameplayEffectContextHandle Context = ActorInfo->AbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(CooldownEffectClass, 1.f, Context);
        if (SpecHandle.IsValid())
        {
            SpecHandle.Data->SetDuration(CooldownDuration, true);
            ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

