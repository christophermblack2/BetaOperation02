// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilitySystemComponent.h"

UMyAbilitySystemComponent::UMyAbilitySystemComponent()
{
	//Multi-player replication	
	SetIsReplicatedByDefault(true);
	// Mixed is a good general default (attibutes replicate; gameplay cue predict well)
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;
}