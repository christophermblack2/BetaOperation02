#pragma once

#include "CoreMinimal.h"
#include "Abilities/GA_BaseSpell.h"
#include "GA_Fireball.generated.h"

/** Fireball ability: spawns ABOFireballSpell and applies cooldown */
UCLASS()
class BETAOPERATION_API UGA_Fireball : public UGA_BaseSpell
{
    GENERATED_BODY()
public:
    UGA_Fireball();
};
