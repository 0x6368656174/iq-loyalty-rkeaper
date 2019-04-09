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

#include "types.h"
#include "lib.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;

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

struct loyalty_cart_data {
  utility::string_t phone;
  INT64 bonusSum;
};

std::string to_utf8 (const std::wstring& str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.to_bytes(str);
}
std::string to_utf8 (const std::string& str)
{
  return str;
}

loyalty_cart_data load_cart(const utility::string_t &phone) {
  BOOST_LOG_TRIVIAL(info) << boost::format("Load loyalty cart for %1%") % to_utf8(phone);

  utility::stringstream_t body;

  auto query = _XPLATSTR("query($shopId: String!, $shopSecret: String!, $clientPhone: String!) {"
                 "  shop(id: $shopId, secret: $shopSecret) {"
                 "    loyaltyCard(phone: $clientPhone) {"
                 "      bonusSum"
                 "    }"
                 "  }"
                 "}");

  json::value request = json::value::object({
                                                {_XPLATSTR("query"), json::value::string(query)},
                                                {_XPLATSTR("operationName"), json::value::null()},
                                                {_XPLATSTR("variables"), json::value::object({
                                                                                         {_XPLATSTR("shopId"), json::value::string(_XPLATSTR("hrS2LU3Z3SLJoVUuBXgk"))},
                                                                                         {_XPLATSTR("shopSecret"), json::value::string(_XPLATSTR("2"))},
                                                                                         {_XPLATSTR("clientPhone"), json::value::string(phone)},
                                                })},
                                            });

  // Create http_client to send the request.
  http_client client(_XPLATSTR("https://us-central1-iq-loyalty-system.cloudfunctions.net/"));

  // Build request URI and start the request.
  uri_builder builder(_XPLATSTR("/api/v1/graphql"));

  loyalty_cart_data result = {phone, 0};

  loyalty_cart_data result = {phone, 0};

  try
  {
    BOOST_LOG_TRIVIAL(trace) << boost::format("GraphQL >>> %1%") % to_utf8(request.serialize());

    client.request(methods::POST, builder.to_string(), request).then([](http_response response) {
      return response.extract_json();
    }).then([&](json::value response) {
      BOOST_LOG_TRIVIAL(trace) << boost::format("GraphQL <<< %1%") % to_utf8(response.serialize());

      result.bonusSum = response.as_object().at(_XPLATSTR("shop")).as_object().at(_XPLATSTR("loyaltyCard")).as_object().at(_XPLATSTR("bonusSum")).as_number().to_int64();
    }).wait();
  }
  catch (const std::exception &e)
  {
    BOOST_LOG_TRIVIAL(error) << boost::format("Error exception: %1%") % e.what();
  }

  return result;
}

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
    WORD outputKind) {
  if (cardNumber < 100000000) {
    return 1;
  }

  if (cardNumber > 999999999) {
    return 1;
  }


//  utility::string_t phone = _XPLATSTR((boost::format("+79%1%") % cardNumber).str());
    utility::string_t phone = _XPLATSTR("+79144191709");

  auto cartData = load_cart(phone);

  cardInfo->exists = 0;
  cardInfo->seizure = 0;
  cardInfo->expired = 0;
  cardInfo->disabled = 0;
  cardInfo->needConfirmation = 0;
  cardInfo->locked = 0;
  cardInfo->ownerNubmer = cardNumber;
  cardInfo->accountNumber = 1;
  cardInfo->defaulterNumber = 1;
  cardInfo->bonusNumber = 19;
  cardInfo->discountNumber = 10;
  cardInfo->maxDiscountSum = 0;
  cardInfo->bonusSum = cartData.bonusSum;
  cardInfo->bonusSum2 = 0;
  cardInfo->bonusSum3 = 0;
  cardInfo->bonusSum4 = 0;
  cardInfo->bonusSum5 = 0;
  cardInfo->bonusSum6 = 0;
  cardInfo->bonusSum7 = 0;
  cardInfo->bonusSum8 = 0;

  return 0;
}
