// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BOCharacter.generated.h"

class UAbilitySystemComponent;
class ABOPlayerState;

/**
 * Character implements IAbilitySystemInterface for convenience, but ASC actually lives on PlayerState.
 * This allows seamless respawn/possession changes in 4-player co-op later.
 */
UCLASS()
class BETAOPERATION_API ABOCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()
public:
    ABOCharacter();

    // IAbilitySystemInterface - forward to PlayerState's ASC
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;          // Server
    virtual void OnRep_PlayerState() override;                              // Client
    // Assign a UMG widget (e.g., WBP_HUD_Player) in the BP child of ABOCharacter
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> HUDWidgetClass;


    UPROPERTY(Transient)
    TObjectPtr<class UUserWidget> HUDWidgetInstance;

private:
    // Helper to (re)initialize ASC Owner/Avatar
    void InitAbilityActorInfo();
    bool bGaveStartupAbilities = false;
};
