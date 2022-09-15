#ifndef EMBDCORECPP_EXPORT_H
#define EMBDCORECPP_EXPORT_H

#ifdef EMBDCORECPP_STATIC
#define EMBDCORECPP_EXPORT
#define EMBDCORECPP_NO_EXPORT
#else
#ifndef EMBDCORECPP_EXPORT
#ifdef EMBDCORECPP_EXPORTS
/* We are building this library */
#define EMBDCORECPP_EXPORT __attribute__((visibility("default")))
#else
/* We are using this library */
#define EMBDCORECPP_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifndef EMBDCORECPP_NO_EXPORT
#define EMBDCORECPP_NO_EXPORT __attribute__((visibility("hidden")))
#endif
#endif

#ifndef EMBDCORECPP_DEPRECATED
#define EMBDCORECPP_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef EMBDCORECPP_DEPRECATED_EXPORT
#define EMBDCORECPP_DEPRECATED_EXPORT EMBDCORECPP_EXPORT EMBDCORECPP_DEPRECATED
#endif

#ifndef EMBDCORECPP_DEPRECATED_NO_EXPORT
#define EMBDCORECPP_DEPRECATED_NO_EXPORT \
    EMBDCORECPP_NO_EXPORT EMBDCORECPP_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#ifndef EMBDCORECPP_NO_DEPRECATED
#define EMBDCORECPP_NO_DEPRECATED
#endif
#endif

#endif /* EMBDCORECPP_EXPORT_H */
