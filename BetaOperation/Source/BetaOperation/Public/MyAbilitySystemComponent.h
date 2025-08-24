// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

/**
 * Custom ASC. Keep lightweight; add helper functions & wrappers as you grow.
 */
UCLASS()
class BETAOPERATION_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UMyAbilitySystemComponent();
};
