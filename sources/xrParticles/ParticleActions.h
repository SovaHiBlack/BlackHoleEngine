//==================================== BlackHoleEngine ============================================
// Projekt		: Particles
// Module		: ParticleActions.h
// Author		: Anahoret
// Description	: 
//================================== SovaHiBlack© - 2022 ==========================================

#pragma once
#ifndef PARTICLES_PARTICLEACTIONS_H_INCLUDED
#define PARTICLES_PARTICLEACTIONS_H_INCLUDED

namespace PAPI
{
	// refs
	struct SParticleEffect;

	struct PARTICLES_API SParticleAction
	{
		enum
		{
			ALLOW_ROTATE					= (1 << 1)
		};

		Flags32								m_Flags;
		EParticleAction							type;	// Type field

		SParticleAction()
		{
			m_Flags.zero();
		}

		virtual void	Execute				(SParticleEffect* pe, f32 dt) = 0;
		virtual void	Transform			(const Fmatrix& m) = 0;

		virtual void	Load				(IReader& F) = 0;
		virtual void	Save				(IWriter& F) = 0;
	};

	DEFINE_VECTOR(SParticleAction*, ParticleActionVec, ParticleActionVec_it);

	class CParticleActions
	{
		ParticleActionVec								actions;

	public:
		CParticleActions()
		{
			actions.reserve(4);
		}
						~CParticleActions()
		{
			clear();
		}
		IC void			clear				()
		{
			for (ParticleActionVec_it it = actions.begin(); it != actions.end(); it++)
			{
				xr_delete(*it);
			}

			actions.clear();
		}
		IC void			append				(SParticleAction* pa)
		{
			actions.push_back(pa);
		}
		IC bool			empty				()
		{
			return actions.empty();
		}
		IC ParticleActionVec_it		begin				()
		{
			return actions.begin();
		}
		IC ParticleActionVec_it		end					()
		{
			return actions.end();
		}
		IC int			size				()
		{
			return actions.size();
		}
		IC void			resize				(int cnt)
		{
			actions.resize(cnt);
		}
//		void			copy				(CParticleActions* src);
	};
};

#endif // ndef PARTICLES_PARTICLEACTIONS_H_INCLUDED