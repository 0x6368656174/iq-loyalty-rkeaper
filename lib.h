#ifndef IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
#define IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H

#include <string>
#include "windows_types.h"
#include "iqloyalty_export.h"
#include "card_info.h"
#include "transaction.h"

extern "C" int IQLOYALTY_EXPORT GetCardInfoEx(
        INT64 card_number,
        DWORD restaurant_code,
        DWORD unit_number,
        card_info* card_info,
        char* input_buffer,
        DWORD input_length,
        WORD input_kind,
        char* output_buffer,
        DWORD output_length,
        WORD output_kind);

extern "C" int IQLOYALTY_EXPORT TransactionsEx(DWORD count,
                                               transaction** transactions,
                                               char* input_buffer,
                                               DWORD input_buffer_length,
                                               WORD input_bufferKind,
                                               char* output_buffer,
                                               DWORD output_buffer_length,
                                               WORD output_buffer_kind);


extern "C" void IQLOYALTY_EXPORT Init();

extern "C" void IQLOYALTY_EXPORT Done();

#endif //IQ_LOYALTY_SYSTEM_RKEEPER_LIB_H
