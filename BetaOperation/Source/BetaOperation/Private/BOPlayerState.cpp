// Fill out your copyright notice in the Description page of Project Settings.


#include "BOPlayerState.h"
#include "MyAbilitySystemComponent.h"
#include "BOHealthAttributeSet.h"

ABOPlayerState::ABOPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystem"));
    HealthSet = CreateDefaultSubobject<UBOHealthAttributeSet>(TEXT("HealthSet"));
}

UAbilitySystemComponent* ABOPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
