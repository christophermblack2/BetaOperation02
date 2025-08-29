#include "Abilities/GA_Fireball.h"
#include "BOFireballSpell.h"
#include "Abilities/GE_SimpleCooldown.h"

UGA_Fireball::UGA_Fireball()
{
    CooldownDuration = 0.5f;
    SpellClass = ABOFireballSpell::StaticClass();
    CooldownEffectClass = UGE_SimpleCooldown::StaticClass();
}
