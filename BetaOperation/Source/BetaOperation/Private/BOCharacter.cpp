// Fill out your copyright notice in the Description page of Project Settings.


#include "BOCharacter.h"
#include "BOPlayerState.h"
#include "MyAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"  // for CreateWidget & AddToViewport
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"



ABOCharacter::ABOCharacter() 
{
    // Camera/rotation defaults
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = true;

    // Movement defaults
    UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    if (MoveComp)
    {
        // If you want “run toward movement direction” style:
        // MoveComp->bOrientRotationToMovement = true; bUseControllerRotationYaw = false;
        MoveComp->RotationRate = FRotator(0.f, 540.f, 0.f);
        MoveComp->JumpZVelocity = 600.f;
        MoveComp->AirControl = 0.35f;
    }
}

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

void ABOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (IA_MoveForward) EIC->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ABOCharacter::MoveForward);
        if (IA_MoveRight) EIC->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ABOCharacter::MoveRight);
        if (IA_Look) EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABOCharacter::Look);
        if (IA_Jump)
        {
            EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
            EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }
    }
}


void ABOCharacter::MoveForward(const FInputActionValue& Value)
{
    const float Axis = Value.Get<float>();
    if (!Controller || FMath::IsNearlyZero(Axis)) return;


    const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
    const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Axis);
}


void ABOCharacter::MoveRight(const FInputActionValue& Value)
{
    const float Axis = Value.Get<float>();
    if (!Controller || FMath::IsNearlyZero(Axis)) return;


    const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
    const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Axis);
}


void ABOCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>();
    if (!Axis.IsNearlyZero())
    {
        AddControllerYawInput(Axis.X);
        AddControllerPitchInput(Axis.Y);
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