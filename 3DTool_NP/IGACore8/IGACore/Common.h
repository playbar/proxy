#pragma once

#include "IGACommon.h"

#define DEFAULT_IGAVFT_SIZE 1024

#define DECLARE_DETOUR_BASECLASS(base_class) class Trampoline_##base_class
#define DECLARE_DETOUR_CLASS(base_class) \
	struct IGA_##base_class * Get##base_class##Proxy(); \
	IGAVFT Get##base_class##SourceVFT(); \
	struct IGA_##base_class 

#define IMPLEMENT_DETOUR_CLASS(base_class) \
	struct IGA_##base_class * Get##base_class##Proxy() { \
		static IGA_##base_class StaticObject; \
		return &StaticObject; \
	} \
	static uint32 s_vft##base_class[DEFAULT_IGAVFT_SIZE]; \
	static IGAVFT s_igavft##base_class = s_vft##base_class; \
	IGAVFT Get##base_class##SourceVFT() { \
		return s_igavft##base_class; \
	}

#define DECLARE_DETOUR_MEMBER_V_STDCALL_NAKED(lret, method_name, method_params) \
    virtual void __stdcall method_name(...)

#define DECLARE_DETOUR_MEMBER_V_STDCALL(lret, method_name, method_params) \
	virtual lret __stdcall method_name##method_params

#define IMPLEMENT_DETOUR_MEMBER_NAKED(interface_name, lret, method_name, method_params) \
	__declspec(naked) void IGA_##interface_name::method_name(...)
/*
#define IMPLEMENT_SOURCE_MEMBER(interface_name, lret, method_name, method_params, vindex) \
	typedef lret (__stdcall* org_##method_name)method_params; \
	static const org_##method_name lpvfn##method_name = (org_##method_name)s_vft##interface_name[vindex];

#define IMPLEMENT_SOURCE_MEMBER(interface_name, lret, method_name, method_params, vindex) \
	typedef lret (__stdcall* org_##method_name)method_params; \
	static const org_##method_name lpvfn##method_name = (org_##method_name)s_vft##interface_name[vindex];
*/
#define DECLARE_SOURCE_MEMBER(interface_name, lret, method_name, method_params, vindex) \
	typedef lret (__stdcall* org_##method_name)method_params; \
	static org_##method_name lpvfn##method_name;

#define IMPLEMENT_SOURCE_MEMBER(interface_name, lret, method_name, method_params, vindex) \
	lpvfn##method_name = (org_##method_name)s_vft##interface_name[vindex];

#define IMPLEMENT_DETOUR_MEMBER(interface_name, lret, method_name, method_params) \
	lret IGA_##interface_name::method_name##method_params

#define IMPLEMENT_DETOUR_TRAMPOLINE(interface_name, lret, method_name, method_params) \
	lret (__stdcall interface_name::* pf##interface_name##Target_##method_name)method_params = interface_name::method_name; \
	lret (__stdcall IGA_##interface_name::* pf##interface_name##Detour_##method_name)method_params = IGA_##interface_name::detour_##method_name; \
	lret (__stdcall IGA_##interface_name::* pf##interface_name##Trampoline_##method_name)method_params = IGA_##interface_name::trampoline_##method_name; \
    DETOUR_TRAMPOLINE_EMPTY(lret IGA_##interface_name::trampoline_##method_name##method_params);

#define IMPLEMENT_DETOUR_MEMBERFUNCTION(interface_name, lret, method_name, method_prototype) \
    DetourFunctionWithEmptyTrampoline(*(PBYTE*)&pf##interface_name##Trampoline_##method_name, \
	*(PBYTE*)&pf##interface_name##Target_##method_name, \
	*(PBYTE*)&pf##interface_name##Detour_##method_name);
