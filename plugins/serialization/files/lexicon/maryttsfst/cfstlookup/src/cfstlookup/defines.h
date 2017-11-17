#ifndef _CFSTLOOKUP_DEFINES_H_
#define _CFSTLOOKUP_DEFINES_H_

#undef CFSTLOOKUP_BEGIN_C_DECLS
#undef CFSTLOOKUP_END_C_DECLS
#ifdef __cplusplus
#  define CFSTLOOKUP_BEGIN_C_DECLS extern "C" {
#  define CFSTLOOKUP_END_C_DECLS }
#else /* !__cplusplus */
#  define CFSTLOOKUP_BEGIN_C_DECLS
#  define CFSTLOOKUP_END_C_DECLS
#endif  /* __cplusplus */

#if defined _WIN32 || defined __CYGWIN__
#  define CFSTLOOKUP_HELPER_DLL_IMPORT __declspec(dllimport)
#  define CFSTLOOKUP_HELPER_DLL_EXPORT __declspec(dllexport)
#  define CFSTLOOKUP_HELPER_DLL_LOCAL
#else
#  if __GNUC__ >= 4
#    define CFSTLOOKUP_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
#    define CFSTLOOKUP_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
#    define CFSTLOOKUP_HELPER_DLL_LOCAL  __attribute__ ((visibility("hidden")))
#  else
#    define CFSTLOOKUP_HELPER_DLL_IMPORT
#    define CFSTLOOKUP_HELPER_DLL_EXPORT
#    define CFSTLOOKUP_HELPER_DLL_LOCAL
#  endif
#endif

#ifdef CFSTLOOKUP_LIBRARIES_EXPORTS
#  ifdef CFSTLOOKUP_SRC
#    define CFSTLOOKUP_API CFSTLOOKUP_HELPER_DLL_EXPORT
#  else
#    define CFSTLOOKUP_API CFSTLOOKUP_HELPER_DLL_IMPORT
#  endif
#  define CFSTLOOKUP_LOCAL CFSTLOOKUP_HELPER_DLL_LOCAL
#else
#  define CFSTLOOKUP_API
#  define CFSTLOOKUP_LOCAL
#endif

#endif
