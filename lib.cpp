#define _TURN_OFF_PLATFORM_STRING 1

#include <iostream>
#include <codecvt>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/filestream.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/format.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "windows_types.h"
#include "lib.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;

std::string to_utf8 (const utility::string_t& str)
{
#ifdef _WIN32
  if( str.empty() ) return std::string();
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0, NULL, NULL);
  std::string strTo( size_needed, 0 );
  WideCharToMultiByte                  (CP_UTF8, 0, &str[0], (int)str.size(), &strTo[0], size_needed, NULL, NULL);
  return strTo;
#else
  // На самом деле ничего не ковертируется, просто чтоб собиралось под Linux
  return str;
#endif
}

std::string to_cp1251 (const std::wstring& str)
{
#ifdef _WIN32
  if( str.empty() ) return std::string();
  int size_needed = WideCharToMultiByte(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0, NULL, NULL);
  std::string strTo( size_needed, 0 );
  WideCharToMultiByte                  (CP_ACP, 0, &str[0], (int)str.size(), &strTo[0], size_needed, NULL, NULL);
  return strTo;
#else
  // На самом деле ничего не ковертируется, просто чтоб собиралось под Linux
  return str;
#endif
}
std::string to_cp1251 (const std::string& str)
{
  return str;
}

void Init()
{
  static bool inited = false;
  if (!inited) {
    inited = true;
    boost::log::add_common_attributes();
    boost::log::add_file_log
       (
          boost::log::keywords::file_name = "iq-loyalty_%N.log",
          boost::log::keywords::open_mode = std::ios_base::app,
          boost::log::keywords::rotation_size = 5 * 1024 * 1024,
          boost::log::keywords::auto_flush = true,
          boost::log::keywords::format =
             (
                boost::log::expressions::stream
                   << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%b %d %Y %H:%M:%S") << " "
                   << boost::log::expressions::attr<boost::log::attributes::current_process_id::value_type>("ProcessID") << ": "
                   << "%local0-" << boost::log::trivial::severity
                   << "-" << std::hex << std::setw(6) << std::setfill('0') << boost::log::expressions::attr< unsigned int >("LineID")
                   << ": " << boost::log::expressions::smessage
             )
       );
  }

  BOOST_LOG_TRIVIAL(info) << "============= INIT LIB =============";
}

void Done() {
  BOOST_LOG_TRIVIAL(info) << "============= DONE LIB =============";
}


int GetCardInfoEx(
   INT64 card_number,
   DWORD restaurant_code,
   DWORD unit_number,
   card_info* card_info,
   char* input_buffer,
   DWORD input_length,
   WORD input_kind,
   char* output_buffer,
   DWORD output_length,
   WORD output_kind) {
  if (card_number < 100000000) {
    return 1;
  }

  if (card_number > 999999999) {
    return 1;
  }
  try
  {

    utility::string_t phone = _XPLATSTR("+79") + std::to_wstring(card_number);

    BOOST_LOG_TRIVIAL(info) << boost::format("Load loyalty cart for %1%") % to_utf8(phone);

    utility::stringstream_t body;

    auto query = _XPLATSTR("query($shopId: String!, $shopSecret: String!, $clientPhone: String!) {"
                           "  shop(id: $shopId, secret: $shopSecret) {"
                           "    loyaltyCard(phone: $clientPhone) {"
                           "      rKeeper {"
                           "        deleted"
                           "        seizure"
                           "        expired"
                           "        disabled"
                           "        needConfirmation"
                           "        blocked"
                           "        blockedMessage"
                           "        ownerName"
                           "        ownerNumber"
                           "        billNumber"
                           "        defaulterType"
                           "        bonusNumber"
                           "        discountNumber"
                           "        maxDiscount"
                           "        bills"
                           "        cardText"
                           "        screenText"
                           "        printText"
                           "      }"
                           "    }"
                           "  }"
                           "}");

    json::value request = json::value::object({
                                                 {_XPLATSTR("query"), json::value::string(query)},
                                                 {_XPLATSTR("operationName"), json::value::null()},
                                                 {_XPLATSTR("variables"), json::value::object({
                                                                                                 {_XPLATSTR("shopId"), json::value::string(_XPLATSTR("yagoda"))},
                                                                                                 {_XPLATSTR("shopSecret"), json::value::string(_XPLATSTR("2"))},
                                                                                                 {_XPLATSTR("clientPhone"), json::value::string(phone)},
                                                                                              })},
                                              });

    // Create http_client to send the request.
    http_client client(_XPLATSTR("https://us-central1-iq-loyalty-system.cloudfunctions.net/"));

    // Build request URI and start the request.
    uri_builder builder(_XPLATSTR("/api/v1/graphql"));

    BOOST_LOG_TRIVIAL(trace) << boost::format("GraphQL >>> %1%") % to_utf8(request.serialize());

    client.request(methods::POST, builder.to_string(), request).then([](http_response response) {
      return response.extract_json();
    }).then([&](json::value response) {
      BOOST_LOG_TRIVIAL(trace) << boost::format("GraphQL <<< %1%") % to_utf8(response.serialize());

      auto loyalty_card_data = response.as_object().at(_XPLATSTR("data")).as_object().at(_XPLATSTR("shop")).as_object().at(_XPLATSTR("loyaltyCard")).as_object();
      auto r_keeper_data = loyalty_card_data.at(_XPLATSTR("rKeeper")).as_object();

      card_info->deleted = r_keeper_data.at(_XPLATSTR("deleted")).as_bool();
      card_info->seizure = r_keeper_data.at(_XPLATSTR("seizure")).as_bool();
      card_info->expired = r_keeper_data.at(_XPLATSTR("expired")).as_bool();
      card_info->disabled = r_keeper_data.at(_XPLATSTR("disabled")).as_bool();
      card_info->need_confirmation = r_keeper_data.at(_XPLATSTR("needConfirmation")).as_bool();
      card_info->blocked = r_keeper_data.at(_XPLATSTR("blocked")).as_bool();

      if (!r_keeper_data.at(_XPLATSTR("blockedMessage")).is_null()) {
        auto blocked_message = to_utf8(r_keeper_data.at(_XPLATSTR("blockedMessage")).as_string());
        blocked_message.copy(card_info->blocked_message, blocked_message.size() + 1);
      }

      auto owner_name = to_cp1251(r_keeper_data.at(_XPLATSTR("ownerName")).as_string());
      owner_name.copy(card_info->owner_name, owner_name.size() + 1);

      card_info->owner_number = r_keeper_data.at(_XPLATSTR("ownerNumber")).as_number().to_int64();
      card_info->bill_number = r_keeper_data.at(_XPLATSTR("billNumber")).as_number().to_uint32();
      card_info->defaulter_type = r_keeper_data.at(_XPLATSTR("defaulterType")).as_number().to_uint32();
      card_info->bonus_number = r_keeper_data.at(_XPLATSTR("bonusNumber")).as_number().to_uint32();
      card_info->discount_number = r_keeper_data.at(_XPLATSTR("discountNumber")).as_number().to_uint32();
      card_info->max_discount_sum = r_keeper_data.at(_XPLATSTR("maxDiscount")).as_number().to_int64();
      card_info->bill1 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(0).as_number().to_int64();
      card_info->bill2 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(1).as_number().to_int64();
      card_info->bill3 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(2).as_number().to_int64();
      card_info->bill4 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(3).as_number().to_int64();
      card_info->bill5 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(4).as_number().to_int64();
      card_info->bill6 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(5).as_number().to_int64();
      card_info->bill7 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(6).as_number().to_int64();
      card_info->bill8 = r_keeper_data.at(_XPLATSTR("bills")).as_array().at(7).as_number().to_int64();

      auto card_text = to_cp1251(r_keeper_data.at(_XPLATSTR("cardText")).as_string());
      card_text.copy(card_info->card_text, card_text.size() + 1);

      auto screen_text = to_cp1251(r_keeper_data.at(_XPLATSTR("screenText")).as_string());
      screen_text.copy(card_info->screen_text, screen_text.size() + 1);

      auto print_text = to_cp1251(r_keeper_data.at(_XPLATSTR("printText")).as_string());
      print_text.copy(card_info->print_text, print_text.size() + 1);
    }).wait();

    BOOST_LOG_TRIVIAL(info) << "Load loyalty cart completed";
  }
  catch (const std::exception &e)
  {
    BOOST_LOG_TRIVIAL(error) << boost::format("Error exception: %1%") % e.what();
    return 1;
  }

  return 0;
}

int TransactionsEx(DWORD count,
                   void* transactions,
                   char* inputBuffer,
                   DWORD inputBufferLength,
                   WORD inputBufferKind,
                   char* outputBuffer,
                   DWORD outputBufferLength,
                   WORD outputBufferKind) {
  return 1;
}
