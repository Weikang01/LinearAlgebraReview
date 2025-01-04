
#ifndef LAR_EXPORT_H
#define LAR_EXPORT_H

#ifdef LAR_STATIC_DEFINE
#  define LAR_EXPORT
#  define LAR_NO_EXPORT
#else
#  ifndef LAR_EXPORT
#    ifdef LAR_EXPORTS
/* We are building this library */
#      define LAR_EXPORT 
#    else
/* We are using this library */
#      define LAR_EXPORT 
#    endif
#  endif

#  ifndef LAR_NO_EXPORT
#    define LAR_NO_EXPORT 
#  endif
#endif

#ifndef LAR_DEPRECATED
#  define LAR_DEPRECATED __declspec(deprecated)
#endif

#ifndef LAR_DEPRECATED_EXPORT
#  define LAR_DEPRECATED_EXPORT LAR_EXPORT LAR_DEPRECATED
#endif

#ifndef LAR_DEPRECATED_NO_EXPORT
#  define LAR_DEPRECATED_NO_EXPORT LAR_NO_EXPORT LAR_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LAR_NO_DEPRECATED
#    define LAR_NO_DEPRECATED
#  endif
#endif

#endif /* LAR_EXPORT_H */
