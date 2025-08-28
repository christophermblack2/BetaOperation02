// Fill out your copyright notice in the Description page of Project Settings.


#include "BOAttributeBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BOHealthAttributeSet.h"


static UAbilitySystemComponent* BO_GetASC(const AActor* Target)
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(const_cast<AActor*>(Target));
}


float UBOAttributeBlueprintLibrary::GetHealth(const AActor* Target)
{
	if (UAbilitySystemComponent* ASC = BO_GetASC(Target))
	{
		return ASC->GetNumericAttribute(UBOHealthAttributeSet::GetHealthAttribute());
	}
	return 0.f;
}


float UBOAttributeBlueprintLibrary::GetMaxHealth(const AActor* Target)
{
	if (UAbilitySystemComponent* ASC = BO_GetASC(Target))
	{
		return ASC->GetNumericAttribute(UBOHealthAttributeSet::GetMaxHealthAttribute());
	}
	return 0.f;
}


float UBOAttributeBlueprintLibrary::GetHealthPercent(const AActor* Target)
{
	const float MaxH = GetMaxHealth(Target);
	return MaxH > 0.f ? GetHealth(Target) / MaxH : 0.f;
}