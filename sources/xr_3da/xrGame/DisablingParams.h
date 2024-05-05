#ifndef GAME_DISABLING_PARAMS_H
#define GAME_DISABLING_PARAMS_H

struct	SOneDDOParams
{
	void				Mul					(float v)	;
	float				velocity						;
	float				acceleration					;
};

struct	SAllDDOParams
{
	void				Reset						()	;
	void				Load			(CInifile* ini)	;
	SOneDDOParams		translational					;
	SOneDDOParams		rotational						;
	u16					L2frames						;
};

struct SAllDDWParams
{
	SAllDDOParams		objects_params					;
	float				reanable_factor					;
};

extern SAllDDWParams	worldDisablingParams			;

#endif // ndef GAME_DISABLING_PARAMS_H
