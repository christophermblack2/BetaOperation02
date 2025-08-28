#include "BOHealthAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectTypes.h" // for full definition of FGameplayEffectModCallbackData


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


// Clamp MaxHealth when it changes; if MaxHealth shrinks, clamp Health down to it.
void UBOHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);


	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(1.f, NewValue); // keep > 0
	}
	else if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}


// After effects apply, clamp Health to [0, MaxHealth]
void UBOHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float CurrentMax = GetMaxHealth();
		const float Clamped = FMath::Clamp(GetHealth(), 0.f, CurrentMax);
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