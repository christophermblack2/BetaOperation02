// Fill out your copyright notice in the Description page of Project Settings.

// Minimal enemy cylinder with GAS health + worldspace health bar
// =========================
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BOEnemyDummy.generated.h"

class UAbilitySystemComponent;
class UBOHealthAttributeSet;
class UWidgetComponent;

UCLASS()
class BETAOPERATION_API ABOEnemyDummy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABOEnemyDummy();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

protected:
	virtual void BeginPlay() override;

	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBOHealthAttributeSet> HealthSet;

	// World-space widget to show health bar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	// Starting values so the dummy has health
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float StartMaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float StartHealth = 100.f;
};
