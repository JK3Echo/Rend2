#pragma once

//
// fx_*.c
//

// NOTENOTE This is not the best, DO NOT CHANGE THESE!
#define	FX_ALPHA_LINEAR		0x00000001
#define	FX_SIZE_LINEAR		0x00000100



// Bryar
void FX_BryarProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BryarAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BryarHitWall( vec3_t origin, vec3_t normal, int weapon, qboolean altFire );
void FX_BryarAltHitWall(vec3_t origin, vec3_t normal, int power, int weapon, qboolean altFire);
void FX_BryarHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);
void FX_BryarAltHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);

// Blaster
void FX_BlasterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BlasterAltFireThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BlasterWeaponHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_BlasterWeaponHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);

// Disruptor
void FX_DisruptorMainShot( vec3_t start, vec3_t end );
void FX_DisruptorAltShot( vec3_t start, vec3_t end, qboolean fullCharge );
void FX_DisruptorAltMiss(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_DisruptorAltHit( vec3_t origin, vec3_t normal, int weapon, qboolean altFire );
void FX_DisruptorHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_DisruptorHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);

// Bowcaster
void FX_BowcasterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BowcasterAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_BowcasterHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_BowcasterHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);

// Heavy Repeater
void FX_RepeaterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_RepeaterAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_RepeaterHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_RepeaterAltHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_RepeaterHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);
void FX_RepeaterAltHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);

// DEMP2
void FX_DEMP2_ProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_DEMP2_HitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_DEMP2_BounceWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_DEMP2_HitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);
void FX_DEMP2_AltDetonate( vec3_t org, float size );

// Golan Arms Flechette
void FX_FlechetteProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );
void FX_FlechetteWeaponHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_FlechetteWeaponHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);
void FX_FlechetteAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon );

// Personal Rocket Launcher
void FX_RocketProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_PulseRocketProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_RocketAltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_PulseRocketAltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_RocketHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_PulseRocketHitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_RocketHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);
void FX_PulseRocketHitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);

//T21
void FX_T21ProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Thermals
void FX_ThermalProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Pulse Grenades
void FX_PulseGrenadeProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);


void FX_CLONEPISTOL_HitWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_CLONEPISTOL_BounceWall(vec3_t origin, vec3_t normal, int weapon, qboolean altFire);
void FX_CLONEPISTOL_HitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid, int weapon, qboolean altFire);
void FX_CLONEPISTOL_ProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
