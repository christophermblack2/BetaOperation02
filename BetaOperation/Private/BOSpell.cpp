#include "BOSpell.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ABOSpell::ABOSpell()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->InitSphereRadius(12.f);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    RootComponent = Sphere;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    VisualMesh->SetupAttachment(RootComponent);
    VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->InitialSpeed = 1000.f;
    Movement->MaxSpeed = 1000.f;
    Movement->ProjectileGravityScale = 0.f;
    Movement->bRotationFollowsVelocity = true;

    InitialLifeSpan = LifeSeconds;

    SetByCallerDamageTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Damage"));
}

void ABOSpell::InitializeSpell(UAbilitySystemComponent* InSourceASC, float InDamage)
{
    SourceASC = MakeWeakObjectPtr(InSourceASC);
    DamageMagnitude = InDamage;
}

void ABOSpell::BeginPlay()
{
    Super::BeginPlay();

    if (CastSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(this, CastSFX, GetActorLocation());
    }

    if (TrailVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAttached(TrailVFX, RootComponent, NAME_None,
            FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
    }

    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABOSpell::OnSphereOverlap);

    if (AActor* OwnerActor = GetOwner())
    {
        Sphere->IgnoreActorWhenMoving(OwnerActor, true);
    }
    if (APawn* InstigatorPawn = GetInstigator())
    {
        Sphere->IgnoreActorWhenMoving(InstigatorPawn, true);
    }
}

void ABOSpell::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == GetInstigator())
    {
        HandleImpact(SweepResult);
        return;
    }

    if (ImpactVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVFX, SweepResult.ImpactPoint);
    }
    if (ImpactSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, SweepResult.ImpactPoint);
    }

    if (DamageEffectClass)
    {
        if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
        {
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

            FGameplayEffectSpecHandle SpecHandle;
            if (SourceASC.IsValid())
            {
                SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, Context);
            }
            else
            {
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

    HandleImpact(SweepResult);
}

void ABOSpell::HandleImpact(const FHitResult& SweepResult)
{
    Destroy();
}

