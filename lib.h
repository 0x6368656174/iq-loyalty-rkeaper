#ifndef IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
#define IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H

#include <string>
#include "types.h"
#include "iqloyalty_export.h"

#pragma pack(1)
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

extern "C" int IQLOYALTY_EXPORT GetCardInfoEx(
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

struct Transaction {

};

extern "C" int IQLOYALTY_EXPORT TransactionsEx(DWORD count,
                                               void* transactions,
                                               char* inputBuffer,
                                               DWORD inputBufferLength,
                                               WORD inputBufferKind,
                                               char* outputBuffer,
                                               DWORD outputBufferLength,
                                               WORD outputBufferKind);


extern "C" void IQLOYALTY_EXPORT Init();

extern "C" void IQLOYALTY_EXPORT Done();

#endif //IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
