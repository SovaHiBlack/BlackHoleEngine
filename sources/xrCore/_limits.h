//==================================== BlackHoleEngine ========================================
// Projekt		: Core
// Module		: _limits.h
// Created		: 
// Modified		: 
// Author		: Anahoret
// Description	: Type limits definitions
//================================== SovaHiBlack© - 2022 ======================================*

#pragma once
#ifndef CORE__LIMITS_H_INCLUDED
#define CORE__LIMITS_H_INCLUDED

// Type limits
#define type_max(T)		(std::numeric_limits<T>::max())
#define type_min(T)		(-std::numeric_limits<T>::max())
#define type_zero(T)	(std::numeric_limits<T>::min())
#define type_epsilon(T)	(std::numeric_limits<T>::epsilon())

#define int_max			type_max(int)
#define int_min			type_min(int)
#define int_zero		type_zero(int)

#define flt_max			type_max(float)
#define flt_min			type_min(float)
#define flt_zero		type_zero(float)
#define flt_eps			type_epsilon(float)

#undef FLT_MAX
#define FLT_MAX			flt_max
#undef FLT_MIN
#define FLT_MIN			flt_min

#define dbl_max			type_max(double)
#define dbl_min			type_min(double)
#define dbl_zero		type_zero(double)
#define dbl_eps			type_epsilon(double)

#endif // ndef CORE__LIMITS_H_INCLUDED