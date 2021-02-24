#ifndef __IPL_COMMON__
#define __IPL_COMMON__

#ifdef IPL_EXPORTS
#define IPL_PORT __declspec(dllexport)
#else
#define IPL_PORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    IPL_PORT int limit_intensity(double intensity);

#ifdef __cplusplus
}
#endif

#endif // __COMMON__
