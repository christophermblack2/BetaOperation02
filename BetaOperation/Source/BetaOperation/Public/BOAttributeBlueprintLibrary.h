// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BOAttributeBlueprintLibrary.generated.h"


class UAbilitySystemComponent;


UCLASS()
class BETAOPERATION_API UBOAttributeBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Attributes", meta = (DefaultToSelf = "Target", CompactNodeTitle = "Health"))
	static float GetHealth(const AActor* Target);


	UFUNCTION(BlueprintPure, Category = "Attributes", meta = (DefaultToSelf = "Target", CompactNodeTitle = "MaxHealth"))
	static float GetMaxHealth(const AActor* Target);


	UFUNCTION(BlueprintPure, Category = "Attributes", meta = (DefaultToSelf = "Target", CompactNodeTitle = "Health%"))
	static float GetHealthPercent(const AActor* Target);
};
