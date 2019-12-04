﻿#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <intrin.h>
#include <cstdint>

#define STATIC_ASSERT(COND,MSG) \
typedef char static_assertion_##MSG[(COND)?1:-1] // 

namespace c2
{
	namespace util
	{
		enum CrashReason
		{
			eNullptr,
			eZeroDevide,
		};

		inline void crash_assert(bool isOk)
		{
			if (isOk)
				return;
#if _M_AMD64
			uint64_t* crash_val = 0;
			*crash_val = 0xDDEEAADDBBEEEEFF;
#else  
			uint32_t* crash_val = 0;
			*crash_val = 0xDEADBEEF;
#endif
		}


		inline void crash()
		{
#if _M_AMD64
			uint64_t* crash_val = 0;
			*crash_val = 0xDDEEAADDBBEEEEFF;
#else  
			uint32_t* crash_val = 0;
			*crash_val = 0xDEADBEEF;
#endif
		}


		//inline void crash_assert(bool isOk, CrashReason cReason)
		//{
		//	if (isOk)
		//		return;

		//	int* crashVal = 0;
		//	*crashVal = 0xDEADBEEF;
		//}

		inline void break_point()
		{
//#ifdef _DEBUG
#if _M_AMD64
			__debugbreak(); // AMD64
#else  
			__asm int 3; // WIN32
#endif
//#endif // _DEBUG
		}


	}

}

#endif