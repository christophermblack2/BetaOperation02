// Fill out your copyright notice in the Description page of Project Settings.


#include "BOCharacter.h"
#include "BOPlayerState.h"
#include "MyAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"  // for CreateWidget & AddToViewport


ABOCharacter::ABOCharacter() {}

UAbilitySystemComponent* ABOCharacter::GetAbilitySystemComponent() const
{
    if (const ABOPlayerState* PS = GetPlayerState<ABOPlayerState>())
    {
        return PS->GetAbilitySystemComponent();
    }
    return nullptr;
}

void ABOCharacter::BeginPlay()
{
    Super::BeginPlay();
    InitAbilityActorInfo();
    if (IsLocallyControlled() && HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }
}

void ABOCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController); // Server path
    InitAbilityActorInfo();
}

void ABOCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState(); // Client path
    InitAbilityActorInfo();
}

void ABOCharacter::InitAbilityActorInfo()
{
    if (ABOPlayerState* PS = GetPlayerState<ABOPlayerState>())
    {
        if (UMyAbilitySystemComponent* ASC = PS->GetMyASC())
        {
            ASC->InitAbilityActorInfo(PS, this); // Owner = PlayerState, Avatar = Character

            // Grant default abilities on the server once
            if (HasAuthority() && !bGaveStartupAbilities)
            {
                for (const TSubclassOf<UGameplayAbility>& AbilityClass : PS->DefaultAbilities)
                {
                    if (AbilityClass)
                    {
                        ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
                    }
                }
                bGaveStartupAbilities = true;
            }
        }
    }
}