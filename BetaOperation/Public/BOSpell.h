#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "BOSpell.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UAbilitySystemComponent;
class UGameplayEffect;
class UNiagaraSystem;
class USoundBase;
class UStaticMeshComponent;

/** Base spell projectile that applies a GameplayEffect on overlap */
UCLASS()
class BETAOPERATION_API ABOSpell : public AActor
{
    GENERATED_BODY()
public:
    ABOSpell();

    /** Call immediately after spawning to provide context and override damage */
    UFUNCTION(BlueprintCallable, Category="Spell")
    void InitializeSpell(UAbilitySystemComponent* InSourceASC, float InDamage = 25.f);

protected:
    virtual void BeginPlay() override;

    /** Called when sphere overlaps another actor */
    UFUNCTION()
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Additional impact handling that child classes can extend (e.g., radial effects) */
    virtual void HandleImpact(const FHitResult& SweepResult);

    // --- Components ---
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<USphereComponent> Sphere;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UProjectileMovementComponent> Movement;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> VisualMesh;

    // --- Spell configuration ---
    UPROPERTY(EditDefaultsOnly, Category="Spell")
    FString SpellName;

    UPROPERTY(EditDefaultsOnly, Category="Spell")
    float ManaCost = 0.f;

    UPROPERTY(EditDefaultsOnly, Category="Spell")
    float Cooldown = 0.f;

    UPROPERTY(EditDefaultsOnly, Category="Spell")
    FGameplayTag Element;

    UPROPERTY(EditDefaultsOnly, Category="Spell")
    FGameplayTagContainer AppliedTags;

    // Damage effect to apply on hit
    UPROPERTY(EditDefaultsOnly, Category="Damage")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FGameplayTag SetByCallerDamageTag;

    UPROPERTY(EditAnywhere, Category="Damage")
    float DamageMagnitude = 25.f;

    // --- VFX/SFX ---
    UPROPERTY(EditDefaultsOnly, Category="VFX")
    TObjectPtr<UNiagaraSystem> TrailVFX;

    UPROPERTY(EditDefaultsOnly, Category="VFX")
    TObjectPtr<UNiagaraSystem> ImpactVFX;

    UPROPERTY(EditDefaultsOnly, Category="SFX")
    TObjectPtr<USoundBase> CastSFX;

    UPROPERTY(EditDefaultsOnly, Category="SFX")
    TObjectPtr<USoundBase> ImpactSFX;

    // Lifetime
    UPROPERTY(EditDefaultsOnly, Category="Spell")
    float LifeSeconds = 3.f;

    // Source ability system component for context
    TWeakObjectPtr<UAbilitySystemComponent> SourceASC;
};

