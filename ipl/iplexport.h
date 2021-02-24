#ifdef IPL_EXPORTS
#define IPL_PORT __declspec(dllexport)
#else
#define IPL_PORT __declspec(dllimport)
#endif
