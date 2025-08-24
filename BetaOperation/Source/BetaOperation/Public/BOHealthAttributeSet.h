// Source/BetaOperation/Public/BOHealthAttributeSet.h
#pragma once


#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BOHealthAttributeSet.generated.h"


UCLASS()
class BETAOPERATION_API UBOHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBOHealthAttributeSet();


	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// ===== Attributes =====
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBOHealthAttributeSet, Health) // If your project uses the 5.6 macro variant, you can also use ATTRIBUTE_ACCESSORS_BASIC


		UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBOHealthAttributeSet, MaxHealth)


protected:
	// Clamp incoming values
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);


	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
};
