#ifndef IQ_LOYALTY_SYSTEM_RKEEPER_IQLOYALTY_EXPORTS_H
#define IQ_LOYALTY_SYSTEM_RKEEPER_IQLOYALTY_EXPORTS_H

#ifdef _WIN32
#ifdef shared_EXPORTS
        #define IQLOYALTY_EXPORT __declspec(dllexport)
    #else
        #define IQLOYALTY_EXPORT __declspec(dllimport)
    #endif
#else
#define IQLOYALTY_EXPORT
#endif

#endif //IQ_LOYALTY_SYSTEM_RKEEPER_IQLOYALTY_EXPORTS_H
