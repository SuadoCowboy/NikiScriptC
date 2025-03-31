#pragma once

#if BUILD_EXPORT == 1
	#if BUILD_SHARED == 1
		#define NIKIAPI __declspec(dllexport)
	#else
		#define NIKIAPI
	#endif
#else
	#if !defined(BUILD_SHARED) && !defined(BUILD_EXPORT) && (defined(_WIN32) || defined(_WIN64))
		#define NIKIAPI __declspec(dllexport)
	#else
		#define NIKIAPI
	#endif
#endif