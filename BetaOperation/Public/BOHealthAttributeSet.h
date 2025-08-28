// Source/BetaOperation/Public/BOHealthAttributeSet.h
#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h" // Correct include for FGameplayEffectModCallbackData
#include "BOHealthAttributeSet.generated.h"


// If the convenience macro isn't provided by your engine setup, define it here.
#ifndef ATTRIBUTE_ACCESSORS
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
#endif


UCLASS()
class BETAOPERATION_API UBOHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBOHealthAttributeSet();


	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// ========================= Attributes =========================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBOHealthAttributeSet, Health)


		UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBOHealthAttributeSet, MaxHealth)


protected:
	// Clamp incoming values
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBOHealthAttributeSet, Health, OldValue); }


	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBOHealthAttributeSet, MaxHealth, OldValue); }
};