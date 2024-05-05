#ifndef GAME_PHYSICSGAMEPARS_H_INCLUDED
#define GAME_PHYSICSGAMEPARS_H_INCLUDED

extern float object_damage_factor;
extern float collide_volume_max;
extern float collide_volume_min;

struct SEffectPars
{
	const static float vel_cret_sound;
	const static float vel_cret_particles;
	const static float vel_cret_wallmark;
};

struct SCharacterEffectPars
{
	const static float vel_cret_sound;
	const static float vel_cret_particles;
	const static float vel_cret_wallmark;
};

void LoadPhysicsGameParams( );

#endif // ndef GAME_PHYSICSGAMEPARS_H_INCLUDED
