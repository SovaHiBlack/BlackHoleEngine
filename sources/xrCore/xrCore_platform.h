//==================================== BlackHoleEngine ========================================
// Projekt		: Core
// Module		: Core_platform.h
// Created		: 
// Modified		: 
// Author		: Anahoret
// Description	: 
//================================== SovaHiBlack© - 2022 ======================================*

#pragma once
#ifndef CORE_CORE_PLATFORM_H_INCLUDED
#define CORE_CORE_PLATFORM_H_INCLUDED

// Чтобы ускорить процесс сборки
#define VC_EXTRALEAN						// | уменьшает размер файлов заголовков Win32,
#define WIN32_LEAN_AND_MEAN					// | исключая некоторые редко используемые API


#ifndef STRICT
#	define STRICT							// Включить строгий синтаксис
#endif // ndef STRICT

#define IDIRECTPLAY2_OR_GREATER				// ?
#define DIRECTINPUT_VERSION			0x0800	//
#define _CRT_SECURE_NO_DEPRECATE			// vc8.0 stuff, don't deprecate several ANSI functions

// windows.h
#ifndef _WIN32_WINNT
#	define _WIN32_WINNT				0x0500
#endif

#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NODRAWTEXT
#define NOMEMMGR
#define NOMETAFILE
#define NOSERVICE
#define NOCOMM
#define NOHELP
#define NOPROFILER
#define NOMCX
#define NOMINMAX
#define DOSWIN32
#define _WIN32_DCOM

#pragma warning(push)
//#pragma warning(disable:4005)
#include <windows.h>
#include <windowsx.h>
#pragma warning(pop)

#endif // ndef CORE_CORE_PLATFORM_H_INCLUDED