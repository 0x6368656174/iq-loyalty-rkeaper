#ifndef IQ_LOYALTY_SYSTEM_RKEEPER_TRANSACTION_H
#define IQ_LOYALTY_SYSTEM_RKEEPER_TRANSACTION_H

#include "windows_types.h"

#pragma pack(push, 1)
struct transaction {
  WORD length;
  INT64 card_number;
  INT64 owner_number;
  DWORD bill_number;
  BYTE operation_type;
  INT64 amount;
  WORD restaurant_code;
  DWORD cash_date;
  BYTE cash_number;
  DWORD check_number;
  INT64 tax1_amount;
  WORD tax1_percent;
  INT64 tax2_amount;
  WORD tax2_percent;
  INT64 tax3_amount;
  WORD tax3_percent;
  INT64 tax4_amount;
  WORD tax4_percent;
  INT64 tax5_amount;
  WORD tax5_percent;
  INT64 tax6_amount;
  WORD tax6_percent;
  INT64 tax7_amount;
  WORD tax7_percent;
  INT64 tax8_amount;
  WORD tax8_percent;
};
#pragma pack(pop)

#endif //IQ_LOYALTY_SYSTEM_RKEEPER_TRANSACTION_H
