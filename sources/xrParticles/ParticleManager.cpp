//==================================== BlackHoleEngine ============================================
// Projekt		: Particles
// Module		: ParticleManager.cpp
// Author		: Anahoret
// Description	: 
//================================== SovaHiBlack© - 2022 ==========================================

#include "stdafx.h"
#pragma hdrstop

#include "ParticleManager.h"
#include "ParticleEffect.h"
#include "ParticleActions_collection.h"

using namespace PAPI;

// system
CParticleManager PM;
PARTICLES_API IParticleManager* PAPI::ParticleManager()
{
	return &PM;
}

// 
CParticleManager::CParticleManager()
{}

CParticleManager::~CParticleManager()
{}

SParticleEffect* CParticleManager::GetEffectPtr(int effect_id)
{
	R_ASSERT(effect_id >= 0 && effect_id < (int)effect_vec.size());
	return effect_vec[effect_id];
}

CParticleActions* CParticleManager::GetActionListPtr(int a_list_num)
{
	R_ASSERT(a_list_num >= 0 && a_list_num < (int)alist_vec.size());
	return alist_vec[a_list_num];
}

// create
int CParticleManager::CreateEffect(u32 max_particles)
{
	int eff_id = -1;
	for (int i = 0; i < (int)effect_vec.size(); i++)
	{
		if (!effect_vec[i])
		{
			eff_id = i;
			break;
		}
	}

	if (eff_id < 0)
	{
		// Couldn't find a big enough gap. Reallocate.
		eff_id = effect_vec.size();
		effect_vec.push_back(0);
	}

	effect_vec[eff_id] = xr_new<SParticleEffect>(max_particles);

	return eff_id;
}

void CParticleManager::DestroyEffect(int effect_id)
{
	R_ASSERT(effect_id >= 0 && effect_id < (int)effect_vec.size());
	xr_delete(effect_vec[effect_id]);
}

int CParticleManager::CreateActionList()
{
	int list_id = -1;
	for (int i = 0; i < (int)alist_vec.size(); i++)
	{
		if (!alist_vec[i])
		{
			list_id = i;
			break;
		}
	}

	if (list_id < 0)
	{
		// Couldn't find a big enough gap. Reallocate.
		list_id = alist_vec.size();
		alist_vec.push_back(0);
	}

	alist_vec[list_id] = xr_new<CParticleActions>();

	return list_id;
}

void CParticleManager::DestroyActionList(int alist_id)
{
	R_ASSERT(alist_id >= 0 && alist_id < (int)alist_vec.size());
	xr_delete(alist_vec[alist_id]);
}

// control
void CParticleManager::PlayEffect(int effect_id, int alist_id)
{
	// Execute the specified action list.
	CParticleActions* pa = GetActionListPtr(alist_id);
	if (pa == nullptr)			// ERROR
	{
		return;
	}
	// Step through all the actions in the action list.
	for (ParticleActionVec_it it = pa->begin(); it != pa->end(); it++)
	{
		switch ((*it)->type)
		{
			case ePAExplosionID:
			{
				static_cast<PAExplosion*>(*it)->age = 0.0f;
			}
			break;
			case ePASourceID:
			{
				static_cast<PASource*>(*it)->m_Flags.set(PASource::flSilent, FALSE);
			}
			break;
			case ePATurbulenceID:
			{
				static_cast<PATurbulence*>(*it)->age = 0.0f;
			}
			break;
		}
	}
}

void CParticleManager::StopEffect(int effect_id, int alist_id, BOOL deffered)
{
	// Execute the specified action list.
	CParticleActions* pa = GetActionListPtr(alist_id);
	if (pa == nullptr)			// ERROR
	{
		return;
	}
	// Step through all the actions in the action list.
	for (ParticleActionVec_it it = pa->begin(); it != pa->end(); it++)
	{
		switch ((*it)->type)
		{
			case ePASourceID:
			{
				static_cast<PASource*>(*it)->m_Flags.set(PASource::flSilent, TRUE);
			}
			break;
		}
	}

	if (!deffered)
	{
		// effect
		SParticleEffect* pe = GetEffectPtr(effect_id);
		pe->p_count = 0;
	}
}

// update&render
void CParticleManager::Update(int effect_id, int alist_id, f32 dt)
{
	SParticleEffect* pe = GetEffectPtr(effect_id);
	CParticleActions* pa = GetActionListPtr(alist_id);
	// Step through all the actions in the action list.
	for (ParticleActionVec_it it = pa->begin(); it != pa->end(); it++)
	{
		(*it)->Execute(pe, dt);
	}
}

void CParticleManager::Render(int effect_id)
{}

void CParticleManager::Transform(int alist_id, const Fmatrix& full, const Fvector& vel)
{
	// Execute the specified action list.
	CParticleActions* pa = GetActionListPtr(alist_id);
	if (pa == nullptr)			// ERROR
	{
		return;
	}

	Fmatrix mT;
	mT.translate(full.c);

	// Step through all the actions in the action list.
	for (ParticleActionVec_it it = pa->begin(); it != pa->end(); it++)
	{
		BOOL r = (*it)->m_Flags.is(SParticleAction::ALLOW_ROTATE);
		const Fmatrix& m = r ? full : mT;
		(*it)->Transform(m);
		switch ((*it)->type)
		{
			case ePASourceID:
			{
				static_cast<PASource*>(*it)->parent_vel = CParticleVector(vel.x, vel.y, vel.z) * static_cast<PASource*>(*it)->parent_motion;
			}
			break;
		}
	}
}

// effect
void CParticleManager::RemoveParticle(int effect_id, u32 p_id)
{
	SParticleEffect* pe = GetEffectPtr(effect_id);
	pe->Remove(p_id);
}

void CParticleManager::SetMaxParticles(int effect_id, u32 max_particles)
{
	SParticleEffect* pe = GetEffectPtr(effect_id);
	pe->Resize(max_particles);
}

void CParticleManager::SetCallback(int effect_id, OnBirthParticleCB b, OnDeadParticleCB d, pvoid owner, u32 param)
{
	SParticleEffect* pe = GetEffectPtr(effect_id);
	pe->b_cb = b;
	pe->d_cb = d;
	pe->owner = owner;
	pe->param = param;
}

void CParticleManager::GetParticles(int effect_id, SParticle*& particles, u32& cnt)
{
	SParticleEffect* pe = GetEffectPtr(effect_id);
	particles = pe->particles;
	cnt = pe->p_count;
}

u32 CParticleManager::GetParticlesCount(int effect_id)
{
	SParticleEffect* pe = GetEffectPtr(effect_id);
	return pe->p_count;
}

// action
SParticleAction* CParticleManager::CreateAction(EParticleAction type)
{
	SParticleAction* pa = nullptr;
	switch (type)
	{
		case ePAAvoidID:
		{
			pa = xr_new<PAAvoid>();
		}
		break;
		case ePABounceID:
		{
			pa = xr_new<PABounce>();
		}
		break;
		case ePACopyVertexBID:
		{
			pa = xr_new<PACopyVertexB>();
		}
		break;
		case ePADampingID:
		{
			pa = xr_new<PADamping>();
		}
		break;
		case ePAExplosionID:
		{
			pa = xr_new<PAExplosion>();
		}
		break;
		case ePAFollowID:
		{
			pa = xr_new<PAFollow>();
		}
		break;
		case ePAGravitateID:
		{
			pa = xr_new<PAGravitate>();
		}
		break;
		case ePAGravityID:
		{
			pa = xr_new<PAGravity>();
		}
		break;
		case ePAJetID:
		{
			pa = xr_new<PAJet>();
		}
		break;
		case ePAKillOldID:
		{
			pa = xr_new<PAKillOld>();
		}
		break;
		case ePAMatchVelocityID:
		{
			pa = xr_new<PAMatchVelocity>();
		}
		break;
		case ePAMoveID:
		{
			pa = xr_new<PAMove>();
		}
		break;
		case ePAOrbitLineID:
		{
			pa = xr_new<PAOrbitLine>();
		}
		break;
		case ePAOrbitPointID:
		{
			pa = xr_new<PAOrbitPoint>();
		}
		break;
		case ePARandomAccelID:
		{
			pa = xr_new<PARandomAccel>();
		}
		break;
		case ePARandomDisplaceID:
		{
			pa = xr_new<PARandomDisplace>();
		}
		break;
		case ePARandomVelocityID:
		{
			pa = xr_new<PARandomVelocity>();
		}
		break;
		case ePARestoreID:
		{
			pa = xr_new<PARestore>();
		}
		break;
		case ePASinkID:
		{
			pa = xr_new<PASink>();
		}
		break;
		case ePASinkVelocityID:
		{
			pa = xr_new<PASinkVelocity>();
		}
		break;
		case ePASourceID:
		{
			pa = xr_new<PASource>();
		}
		break;
		case ePASpeedLimitID:
		{
			pa = xr_new<PASpeedLimit>();
		}
		break;
		case ePATargetColorID:
		{
			pa = xr_new<PATargetColor>();
		}
		break;
		case ePATargetSizeID:
		{
			pa = xr_new<PATargetSize>();
		}
		break;
		case ePATargetRotateID:
		{
			pa = xr_new<PATargetRotate>();
		}
		break;
		case ePATargetRotateDID:
		{
			pa = xr_new<PATargetRotate>();
		}
		break;
		case ePATargetVelocityID:
		{
			pa = xr_new<PATargetVelocity>();
		}
		break;
		case ePATargetVelocityDID:
		{
			pa = xr_new<PATargetVelocity>();
		}
		break;
		case ePAVortexID:
		{
			pa = xr_new<PAVortex>();
		}
		break;
		case ePATurbulenceID:
		{
			pa = xr_new<PATurbulence>();
		}
		break;
		case ePAScatterID:
		{
			pa = xr_new<PAScatter>();
		}
		break;
		default:
		{
			NODEFAULT;
		}
	}

	pa->type = type;
	return pa;
}
u32 CParticleManager::LoadActions(int alist_id, IReader& R)
{
	// Execute the specified action list.
	CParticleActions* pa = GetActionListPtr(alist_id);
	pa->clear();
	if (R.length())
	{
		u32 cnt = R.r_u32();
		for (u32 k = 0; k < cnt; k++)
		{
			SParticleAction* act = CreateAction((EParticleAction)R.r_u32());
			act->Load(R);
			pa->append(act);
		}
	}

	return pa->size();
}

void CParticleManager::SaveActions(int alist_id, IWriter& W)
{
	// Execute the specified action list.
	CParticleActions* pa = GetActionListPtr(alist_id);
	W.w_u32(pa->size());
	for (ParticleActionVec_it it = pa->begin(); it != pa->end(); it++)
	{
		(*it)->Save(W);
	}
}
