#ifndef IQ_LOYALTY_SYSTEM_RKEEPER_CARD_INFO_H
#define IQ_LOYALTY_SYSTEM_RKEEPER_CARD_INFO_H

#include "windows_types.h"

#pragma pack(push, 1)
struct card_info {
  WORD length;
  BYTE deleted;
  BYTE seizure;
  BYTE expired;
  BYTE disabled;
  BYTE need_confirmation;
  BYTE blocked;
  CHAR blocked_message [256];
  CHAR owner_name [40];
  INT64 owner_number;
  DWORD bill_number;
  DWORD defaulter_type;
  WORD bonus_number;
  WORD discount_number;
  INT64 max_discount_sum;
  INT64 bill1;
  INT64 bill2;
  INT64 bill3;
  INT64 bill4;
  INT64 bill5;
  INT64 bill6;
  INT64 bill7;
  INT64 bill8;
  CHAR card_text [256];
  CHAR screen_text [256];
  CHAR print_text [256];
};
#pragma pack(pop)

#endif //IQ_LOYALTY_SYSTEM_RKEEPER_CARD_INFO_H
