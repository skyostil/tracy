
#if defined(_WIN32) && !defined(__SYMBIAN32__)
		
	typedef float           khronos_float_t; /*_t*/
	typedef signed char     khronos_int8_t;
	typedef unsigned char   khronos_uint8_t;
	typedef short           khronos_int16_t;
	typedef int             khronos_int32_t;
	typedef unsigned int    khronos_uint32_t;

#elif defined(__SYMBIAN32__)
	
	typedef float           khronos_float_t;
	typedef signed char     khronos_int8_t;
	typedef unsigned char   khronos_uint8_t;
	typedef short           khronos_int16_t;
	typedef int             khronos_int32_t;
	typedef unsigned int    khronos_uint32_t;

#else
	/*#error "Unsupported platform in khronos_types.h"*/
	
	typedef float           khronos_float_t;
	typedef signed char     khronos_int8_t;
	typedef unsigned char   khronos_uint8_t;
	typedef short           khronos_int16_t;
	typedef int             khronos_int32_t;
	typedef unsigned int    khronos_uint32_t;
	
#endif

/*
#define VG_API_CALL		__declspec(dllexport)
#define VGU_API_CALL	__declspec(dllexport)
*/
