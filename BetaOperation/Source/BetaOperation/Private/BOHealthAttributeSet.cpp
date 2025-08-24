#include "BOHealthAttributeSet.h"
#include "Net/UnrealNetwork.h"


UBOHealthAttributeSet::UBOHealthAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}


void UBOHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME_CONDITION_NOTIFY(UBOHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBOHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}


// Ensure MaxHealth never goes below 1; optionally pre-clamp Health to new Max
void UBOHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);


	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(1.f, NewValue);
	}
	else if (Attribute == GetHealthAttribute())
	{
		// Pre-clamp to current Max so UI previews look right when designers tweak values
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}


// After any GE executes, enforce 0..Max bounds
void UBOHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float Clamped = FMath::Clamp(GetHealth(), 0.f, GetMaxHealth());
		SetHealth(Clamped);
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		const float NewMax = FMath::Max(1.f, GetMaxHealth());
		SetMaxHealth(NewMax);
		if (GetHealth() > NewMax)
		{
			SetHealth(NewMax);
		}
	}
}


void UBOHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBOHealthAttributeSet, Health, OldValue);
}


void UBOHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBOHealthAttributeSet, MaxHealth, OldValue);
}