//==================================== BlackHoleEngine ============================================
// Projekt		: Particles
// Module		: ParticleDomain.h
// Author		: Anahoret
// Description	: 
//================================== SovaHiBlack© - 2022 ==========================================

#pragma once
#ifndef PARTICLES_PARTICLEDOMAIN_H_INCLUDED
#define PARTICLES_PARTICLEDOMAIN_H_INCLUDED

#define SQRT2PI				2.506628274631000502415765284811045253006f
#define ONEOVERSQRT2PI		(1.0f / SQRT2PI)

namespace PAPI
{
#pragma pack(push,4)
	struct PARTICLES_API SParticleDomain
	{
		// PABoxDomain, PASphereDomain, PAConeDomain...
		EParticleDomain					type;

		// Box vertices, Sphere center, Cylinder/Cone ends
		CParticleVector					p1;
		CParticleVector					p2;

		// Orthonormal basis vectors for Cylinder/Cone
		CParticleVector					u;
		CParticleVector					v;

		f32								radius1;		// Outer radius
		f32								radius2;		// Inner radius
		f32								radius1Sqr;		// Used for fast Within test of spheres,
		f32								radius2Sqr;		// and for mag. of u and v vectors for plane.

		BOOL		Within				(const CParticleVector&) const;
		void		Generate			(CParticleVector&) const;

		// transformation
		void		transform			(const SParticleDomain& domain, const Fmatrix& m);
		void		transform_dir		(const SParticleDomain& domain, const Fmatrix& m);

		// This constructor is used when default constructing a SParticleAction that has a pDomain.
		IC			SParticleDomain		()
		{}

		// Construct a domain in the standard way.
					SParticleDomain		(EParticleDomain dtype,
										 f32 a0 = 0.0f, f32 a1 = 0.0f, f32 a2 = 0.0f,
										 f32 a3 = 0.0f, f32 a4 = 0.0f, f32 a5 = 0.0f,
										 f32 a6 = 0.0f, f32 a7 = 0.0f, f32 a8 = 0.0f);
	};
#pragma pack(pop)

	// misc
	f32				NRand				(f32 sigma = 1.0f);
};

#endif // ndef PARTICLES_PARTICLEDOMAIN_H_INCLUDED