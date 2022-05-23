//==================================== BlackHoleEngine ============================================
// Projekt		: Particles
// Module		: ParticleEffect.h
// Author		: Anahoret
// Description	: 
//================================== SovaHiBlack© - 2022 ==========================================

#pragma once
#ifndef PARTICLES_PARTICLEEFFECT_H_INCLUDED
#define PARTICLES_PARTICLEEFFECT_H_INCLUDED

namespace PAPI
{
	// A effect of particles - Info and an array of Particles
	struct SParticleEffect
	{
		u32						p_count;				// Number of particles currently existing.
		u32						max_particles;			// Max particles allowed in effect.
		u32						particles_allocated;	// Actual allocated size.
		SParticle*				particles;				// Actually, num_particles in size
		OnBirthParticleCB		b_cb;
		OnDeadParticleCB		d_cb;
		pvoid					owner;
		u32						param;

	public:
		SParticleEffect(int mp)
		{
			owner					= 0;
			param					= 0;
			b_cb					= 0;
			d_cb					= 0;
			p_count					= 0;
			max_particles			= mp;
			particles_allocated		= max_particles;
			particles				= xr_alloc<SParticle>(max_particles);
		}
		~SParticleEffect()
		{
			xr_free(particles);
		}
		IC int		Resize(u32 max_count)
		{
			// Reducing max.
			if (particles_allocated >= max_count)
			{
				max_particles		= max_count;

				// May have to kill particles.
				if (p_count > max_particles)
				{
					p_count			= max_particles;
				}

				return max_count;
			}

			// Allocate particles.
			SParticle* new_particles = xr_alloc<SParticle>(max_count);
			if (new_particles == nullptr)
			{
				// ERROR - Not enough memory. Just give all we've got.
				max_particles		= particles_allocated;
				return max_particles;
			}

			CopyMemory				(new_particles, particles, p_count * sizeof(SParticle));
			xr_free					(particles);
			particles				= new_particles;

			max_particles			= max_count;
			particles_allocated		= max_count;
			return max_count;
		}
		IC void		Remove(int i)
		{
			if (0 == p_count)
			{
				return;
			}

			SParticle& m = particles[i];
			if (d_cb)
			{
				d_cb(owner, param, m, i);
			}

			m = particles[--p_count]; // íå ìåíÿòü ïðàâèëî óäàëåíèÿ !!! (dependence ParticleGroup)
		}

		IC BOOL		Add(const CParticleVector& pos, const CParticleVector& posB,
						const CParticleVector& size, const CParticleVector& rot, const CParticleVector& vel, u32 color,
						const f32 age = 0.0f, u16 frame = 0, u16 flags = 0)
		{
			if (p_count >= max_particles)
			{
				return FALSE;
			}
			else
			{
				SParticle& P	= particles[p_count];
				P.pos			= pos;
				P.posB			= posB;
				P.size			= size;
				P.rot			= rot;
				P.vel			= vel;
				P.color			= color;
				P.age			= age;
				P.frame			= frame;
				P.flags.assign	(flags);
				if (b_cb)
				{
					b_cb(owner, param, P, p_count);
				}

				p_count++;
				return TRUE;
			}
		}
	};
};

#endif // ndef PARTICLES_PARTICLEEFFECT_H_INCLUDED