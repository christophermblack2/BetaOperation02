// Fill out your copyright notice in the Description page of Project Settings.

#include "BOProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

ABOProjectile::ABOProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(12.f);
	Sphere->SetCollisionProfileName(TEXT("Projectile")); // Set up in Project Settings ? Collision (or use BlockAllDynamic to start)
	Sphere->SetNotifyRigidBodyCollision(true);
	RootComponent = Sphere;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
	VisualMesh->SetupAttachment(RootComponent);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = LifeSeconds;

	// Default the set-by-caller tag
	SetByCallerDamageTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Damage"));
}

void ABOProjectile::InitializeProjectile(UAbilitySystemComponent* InSourceASC, float InDamage)
{
	SourceASC = MakeWeakObjectPtr(InSourceASC);   // <- avoids the assignment issue
	DamageMagnitude = InDamage;
}

void ABOProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentHit.AddDynamic(this, &ABOProjectile::OnSphereHit);

	// If no SourceASC provided, try to grab it from the owner automatically
	if (!SourceASC.IsValid())
	{
		if (AActor* O = GetOwner())
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(O))
			{
				SourceASC = MakeWeakObjectPtr(ASC);
			}
		}
	}
}

void ABOProjectile::OnSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetInstigator())
	{
		Destroy();
		return;
	}

	// Try to get target ASC from the hit actor
	if (DamageEffectClass)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Build an effect context. Prefer the source ASC if available, otherwise allocate a generic one.
			FGameplayEffectContextHandle Context;
			if (SourceASC.IsValid())
			{
				Context = SourceASC->MakeEffectContext();
				Context.AddInstigator(GetInstigator(), GetInstigator());
			}
			else
			{
				Context = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
				Context.AddInstigator(GetInstigator(), GetInstigator());
			}

			// Create an outgoing spec (use level 1 for now)
			FGameplayEffectSpecHandle SpecHandle;
			if (SourceASC.IsValid())
			{
				SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, /*Level*/1.f, Context);
			}
			else
			{
				// Fallback: create a temp ASC-less spec by borrowing the target ASC's allocator
				SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.f, Context);
			}

			if (SpecHandle.IsValid())
			{
				if (SetByCallerDamageTag.IsValid())
				{
					SpecHandle.Data->SetSetByCallerMagnitude(SetByCallerDamageTag, DamageMagnitude);
				}

				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	Destroy();
}