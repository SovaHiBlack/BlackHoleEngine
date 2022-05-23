//==================================== BlackHoleEngine ============================================
// Projekt		: Particles
// Module		: psystem.h
// Author		: Anahoret
// Description	: 
//================================== SovaHiBlack© - 2022 ==========================================

#pragma once
#ifndef PARTICLES_PSYSTEM_H_INCLUDED
#define PARTICLES_PSYSTEM_H_INCLUDED

#ifdef PARTICLES_EXPORTS
#	define PARTICLES_API		__declspec(dllexport)
#else
#	define PARTICLES_API		__declspec(dllimport)
#	pragma comment(lib, "xrParticles.lib")
#endif

// Actually this must be < sqrt(MAXFLOAT) since we store this value squared.
#define P_MAXFLOAT				1.0e16f
#define drand48()				::Random.randF()

namespace PAPI
{
	class CParticleVector : public Fvector
	{
	public:
		IC			CParticleVector(f32 ax, f32 ay, f32 az)
		{
			set(ax, ay, az);
		}
		IC			CParticleVector()
		{}
		IC f32		length		() const
		{
			return _sqrt(x * x + y * y + z * z);
		}
		IC f32		length2		() const
		{
			return (x * x + y * y + z * z);
		}
		IC f32		operator*	(const CParticleVector& a) const
		{
			return x * a.x + y * a.y + z * a.z;
		}
		IC CParticleVector	operator*	(const f32 s) const
		{
			return CParticleVector(x * s, y * s, z * s);
		}
		IC CParticleVector	operator/	(const f32 s) const
		{
			f32 invs = 1.0f / s;
			return CParticleVector(x * invs, y * invs, z * invs);
		}
		IC CParticleVector	operator+	(const CParticleVector& a) const
		{
			return CParticleVector(x + a.x, y + a.y, z + a.z);
		}
		IC CParticleVector	operator-	(const CParticleVector& a) const
		{
			return CParticleVector(x - a.x, y - a.y, z - a.z);
		}
		IC CParticleVector	operator-	()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}
		IC CParticleVector& operator+=	(const CParticleVector& a)
		{
			x += a.x;
			y += a.y;
			z += a.z;
			return *this;
		}
		IC CParticleVector& operator-=	(const CParticleVector& a)
		{
			x -= a.x;
			y -= a.y;
			z -= a.z;
			return *this;
		}
		IC CParticleVector& operator*=	(const f32 a)
		{
			x *= a;
			y *= a;
			z *= a;
			return *this;
		}
		IC CParticleVector& operator/=	(const f32 a)
		{
			f32 b = 1.0f / a;
			x *= b;
			y *= b;
			z *= b;
			return *this;
		}
		IC CParticleVector& operator=	(const CParticleVector& a)
		{
			x = a.x;
			y = a.y;
			z = a.z;
			return *this;
		}
		IC CParticleVector	operator^	(const CParticleVector& b) const
		{
			return CParticleVector(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
		}
	};

	// A single particle
	struct SParticle
	{
		enum
		{
			ANIMATE_CCW = (1 << 0)
		};

		CParticleVector		pos;	// 12
		CParticleVector		posB;	// 12
		CParticleVector		vel;	// 12
		CParticleVector		size;	// 12
		CParticleVector		rot;	// 12		60
		u32			color;	// 4
		f32			age;	// 4
		u16			frame;	// 2
		Flags16		flags;	// 2
	};						//			72

	typedef void (*OnBirthParticleCB)	(void* owner, u32 param, PAPI::SParticle& P, u32 idx);
	typedef void (*OnDeadParticleCB)	(void* owner, u32 param, PAPI::SParticle& P, u32 idx);

	//////////////////////////////////////////////////////////////////////
	// Type codes for domains
	enum EParticleDomain
	{
		ePDPoint				= 0,		// Single point
		ePDLine					= 1,		// Line segment
		ePDTriangle				= 2,		// Triangle
		ePDPlane				= 3,		// Arbitrarily-oriented plane
		ePDBox					= 4,		// Axis-aligned box
		ePDSphere				= 5,		// Sphere
		ePDCylinder				= 6,		// Cylinder
		ePDCone					= 7,		// Cone
		ePDBlob					= 8,		// Gaussian blob
		ePDDisc					= 9,		// Arbitrarily-oriented disc
		ePDRectangle			= 10,		// Rhombus-shaped planar region
		ePD_forcedword			= u32(-1)
	};

	//////////////////////////////////////////////////////////////////////
	// Type codes for all actions
	enum EParticleAction
	{
		ePAAvoidID,							// Avoid entering the domain of space.
		ePABounceID,						// Bounce particles off a domain of space.
		PACallActionListID_obsolette,		// íèãäå íå èñïîëüçóåòñÿ
		ePACopyVertexBID,					// Set the secondary position from current position.
		ePADampingID,						// Dampen particle velocities.
		ePAExplosionID,						// An Explosion.
		ePAFollowID,						// Accelerate toward the previous particle in the effect.
		ePAGravitateID,						// Accelerate each particle toward each other particle.
		ePAGravityID,						// Acceleration in the given direction.
		ePAJetID,							// 
		ePAKillOldID,						// 
		ePAMatchVelocityID,					// 
		ePAMoveID,							// 
		ePAOrbitLineID,						// 
		ePAOrbitPointID,					// 
		ePARandomAccelID,					// 
		ePARandomDisplaceID,				// 
		ePARandomVelocityID,				// 
		ePARestoreID,						// 
		ePASinkID,							// 
		ePASinkVelocityID,					// 
		ePASourceID,						// 
		ePASpeedLimitID,					// 
		ePATargetColorID,					// 
		ePATargetSizeID,					// 
		ePATargetRotateID,					// 
		ePATargetRotateDID,					// 
		ePATargetVelocityID,				// 
		ePATargetVelocityDID,				// 
		ePAVortexID,						// 
		ePATurbulenceID,					// 
		ePAScatterID,						// 
		ePA_forcedword = u32(-1)
	};

	struct SParticleAction;

	class IParticleManager
	{
	public:
									IParticleManager	()
		{}
		virtual						~IParticleManager	()
		{}

		// create&destroy
		virtual int					CreateEffect		(u32 max_particles) = 0;
		virtual void				DestroyEffect		(int effect_id) = 0;
		virtual int					CreateActionList	() = 0;
		virtual void				DestroyActionList	(int alist_id) = 0;

		// control
		virtual void				PlayEffect			(int effect_id, int alist_id) = 0;
		virtual void				StopEffect			(int effect_id, int alist_id, BOOL deffered = TRUE) = 0;

		// update&render
		virtual void				Update				(int effect_id, int alist_id, f32 dt) = 0;
		virtual void				Render				(int effect_id) = 0;
		virtual void				Transform			(int alist_id, const Fmatrix& m, const Fvector& velocity) = 0;

		// effect
		virtual void				RemoveParticle		(int effect_id, u32 p_id) = 0;
		virtual void				SetMaxParticles		(int effect_id, u32 max_particles) = 0;
		virtual void				SetCallback			(int effect_id, OnBirthParticleCB b, OnDeadParticleCB d, void* owner, u32 param) = 0;
		virtual void				GetParticles		(int effect_id, SParticle*& particles, u32& cnt) = 0;
		virtual u32					GetParticlesCount	(int effect_id) = 0;

		// action
		virtual SParticleAction*		CreateAction		(EParticleAction type) = 0;
		virtual u32					LoadActions			(int alist_id, IReader& R) = 0;
		virtual void				SaveActions			(int alist_id, IWriter& W) = 0;
	};

	PARTICLES_API IParticleManager*	ParticleManager		();
};

#endif // ndef PARTICLES_PSYSTEM_H_INCLUDED