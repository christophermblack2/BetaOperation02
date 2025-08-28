// Fill out your copyright notice in the Description page of Project Settings.
// Simple projectile that applies a GameplayEffect to hit actors (GAS)
// =========================
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "BOProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class BETAOPERATION_API ABOProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABOProjectile();

	// Call this right after spawning
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void InitializeProjectile(UAbilitySystemComponent* InSourceASC, float InDamage = -25.f);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);


protected:
	// --- Components ---
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> VisualMesh;

	// --- Damage config ---
	// If provided, this GameplayEffect will be applied to the hit target.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Optional SetByCaller tag to drive magnitude from C++ (e.g., Data.Damage)
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag SetByCallerDamageTag;

	// Raw damage fallback if your GE uses SetByCaller magnitude
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageMagnitude = 25.f;

	// Lifetime so projectiles auto-despawn
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSeconds = 5.f;

	// Weak pointer to the source ASC for better effect context (optional)
	TWeakObjectPtr<UAbilitySystemComponent> SourceASC;
};
