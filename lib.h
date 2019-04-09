#ifndef IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
#define IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H

#include <string>
#include "types.h"

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
    INT64 discountSum;
    INT64 discountSum2;
    INT64 discountSum3;
    INT64 discountSum4;
    INT64 discountSum5;
    INT64 discountSum6;
    INT64 discountSum7;
    INT64 discountSum8;
    char cardInfo [256];
    char screenInfo [256];
    char printInfo [256];
};

int GetCardInfoEx(
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

void load_cart(const std::string &phone);

#endif //IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
