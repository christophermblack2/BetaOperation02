// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "BOPlayerState.generated.h"

class UMyAbilitySystemComponent;
class UBOHealthAttributeSet;
class UGameplayAbility;

/**
 * GAS-on-PlayerState pattern (best for multiplayer & respawn). The Character becomes the Avatar.
 */
UCLASS()
class BETAOPERATION_API ABOPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()
public:
    ABOPlayerState();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    UMyAbilitySystemComponent* GetMyASC() const { return AbilitySystemComponent; }

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    UBOHealthAttributeSet* GetHealthSet() const { return HealthSet; }

    /** Abilities to auto-grant on server after ASC init (assign in BP) */
    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TObjectPtr<UMyAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TObjectPtr<UBOHealthAttributeSet> HealthSet;
};