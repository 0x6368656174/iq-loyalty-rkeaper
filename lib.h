#ifndef IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
#define IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H

#include <string>
#include "types.h"
#include "iqloyalty_export.h"

struct CardInfo {
    WORD length;
    BYTE exists;
    BYTE seizure;
    BYTE expired;
    BYTE disabled;
    BYTE needConfirmation;
    BYTE locked;
    char lockedMessage [256];
    char ownerName [40];
    INT64 ownerNubmer;
    DWORD accountNumber;
    DWORD defaulterNumber;
    WORD bonusNumber;
    WORD discountNumber;
    INT64 maxDiscountSum;
    INT64 bonusSum;
    INT64 bonusSum2;
    INT64 bonusSum3;
    INT64 bonusSum4;
    INT64 bonusSum5;
    INT64 bonusSum6;
    INT64 bonusSum7;
    INT64 bonusSum8;
    char cardInfo [256];
    char screenInfo [256];
    char printInfo [256];
};

IQLOYALTY_EXPORT int GetCardInfoEx(
        INT64 cardNumber,
        DWORD restaurantCode,
        DWORD unitNumber,
        CardInfo* cardInfo,
        char* inputBuffer,
        DWORD inputLength,
        WORD inputKind,
        char* outputBuffer,
        DWORD outputLength,
        WORD outputKind);

IQLOYALTY_EXPORT void Init();

IQLOYALTY_EXPORT void Done();

#endif //IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
