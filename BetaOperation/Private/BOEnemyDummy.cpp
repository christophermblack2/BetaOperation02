// Fill out your copyright notice in the Description page of Project Settings.

#include "BOEnemyDummy.h"
#include "AbilitySystemComponent.h"
#include "BOHealthAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

ABOEnemyDummy::ABOEnemyDummy()
{
	PrimaryActorTick.bCanEverTick = false;

	// Make the body a simple cylinder-esque capsule
	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UBOHealthAttributeSet>(TEXT("HealthSet"));

	// Health bar widget in world space above the head
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(120.f, 12.f));
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	HealthBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// No mesh/anim required; this is a target dummy.
}

void ABOEnemyDummy::BeginPlay()
{
	Super::BeginPlay();

	// Initialize starting attributes (server side in real game)
	if (HealthSet)
	{
		HealthSet->InitMaxHealth(StartMaxHealth);
		HealthSet->InitHealth(FMath::Clamp(StartHealth, 0.f, StartMaxHealth));
	}
}