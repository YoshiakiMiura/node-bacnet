#include <iostream>
#include <v8.h>
#include <nan.h>
#include "BacnetValue.h"
#include "functions.h"
#include "init.h"
#include "conversion.h"

using v8::Local;
using v8::Value;
using v8::Object;
using v8::Int32;
using v8::Uint32;
using v8::String;
using v8::FunctionTemplate;
using Nan::MaybeLocal;
using Nan::New;

void setTagEnum(Local<Object> target);
void setObjectTypeEnum(Local<Object> target);
void setEnum(Local<Object> target);


// Configures and returns a bacnet instance - currently due to a lot of static c code, only one instance can exist at a time
NAN_METHOD(InitInstance) {
    Local<Object> configJs;
    if (info.Length() > 0 && !info[0]->IsUndefined()) {
        configJs = info[0]->ToObject();
    } else {
        configJs = New<Object>();
    }

    uint32_t device_instance_id = getUint32Default(configJs, "device_instance_id", 0);
    uint16_t ip_port = getUint32Default(configJs, "ip_port", 0xBAC0);
    uint16_t apdu_timeout = getUint32Default(configJs, "apdu_timeout", 3000);
    uint8_t apdu_retries = getUint32Default(configJs, "apdu_retries", 3);
    std::string iface = getStringOrEmpty(configJs, "iface");
    uint8_t invoke_id = getUint32Default(configJs, "invoke_id", 0);
    uint32_t bbmd_port = getUint32Default(configJs, "bbmd_port", 0xBAC0);
    uint32_t bbmd_ttl = getUint32Default(configJs, "bbmd_ttl", 0);
    std::string bbmd_address = getStringOrEmpty(configJs, "bbmd_address");

    struct BACNET_CONFIGURATION config = {device_instance_id, ip_port, apdu_timeout, apdu_retries, iface.c_str(), invoke_id, bbmd_port, bbmd_ttl, bbmd_address.c_str()};
    const char * errorMessage = init_bacnet(&config);
    if (errorMessage) {
        Nan::ThrowError(errorMessage);
        return;
    }

    Local<Object> target = New<Object>();
    Nan::Set(target, New("whois").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(whois)).ToLocalChecked());
    Nan::Set(target, New("isBound").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(isBound)).ToLocalChecked());
    Nan::Set(target, New("readProperty").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(readProperty)).ToLocalChecked());
    Nan::Set(target, New("writeProperty").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(writeProperty)).ToLocalChecked());
    Nan::Set(target, New("listen").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(listen)).ToLocalChecked());
    Nan::Set(target, New("initClient").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(initClient)).ToLocalChecked());
    Nan::Set(target, New("initDevice").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(initDevice)).ToLocalChecked());
    Nan::Set(target, New("closeQueue").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(closeQueue)).ToLocalChecked());
    Nan::Set(target, New("subscribeCov").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(subscribeCov)).ToLocalChecked());
    Nan::Set(target, New("timeSync").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(timeSync)).ToLocalChecked());

    setEnum(target);

    info.GetReturnValue().Set(target);
}

NAN_MODULE_INIT(InitModule) {
    Nan::Set(target, New("init").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(InitInstance)).ToLocalChecked());
    Nan::Set(target, New("objectTypeToString").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(objectTypeToString)).ToLocalChecked());
    Nan::Set(target, New("objectTypeToNumber").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(objectTypeToNumber)).ToLocalChecked());
    Nan::Set(target, New("propertyKeyToString").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(propertyKeyToString)).ToLocalChecked());
    Nan::Set(target, New("propertyKeyToNumber").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(propertyKeyToNumber)).ToLocalChecked());

    BacnetValue::Init(target);
}

NODE_MODULE(binding, InitModule)

void setEnum(Local<Object> target) {
  setObjectTypeEnum(target);
  setTagEnum(target);
}

void setObjectTypeEnum(Local<Object> target) {
  Local<Object> types = New<Object>();
  Nan::Set(types, New("OBJECT_ANALOG_INPUT").ToLocalChecked(),
      New(OBJECT_ANALOG_INPUT));
  Nan::Set(types, New("OBJECT_ANALOG_OUTPUT").ToLocalChecked(),
      New(OBJECT_ANALOG_OUTPUT));
  Nan::Set(types, New("OBJECT_ANALOG_VALUE").ToLocalChecked(),
      New(OBJECT_ANALOG_VALUE));
  Nan::Set(types, New("OBJECT_BINARY_INPUT").ToLocalChecked(),
      New(OBJECT_BINARY_INPUT));
  Nan::Set(types, New("OBJECT_BINARY_OUTPUT").ToLocalChecked(),
      New(OBJECT_BINARY_OUTPUT));
  Nan::Set(types, New("OBJECT_BINARY_VALUE").ToLocalChecked(),
      New(OBJECT_BINARY_VALUE));
  Nan::Set(types, New("OBJECT_CALENDAR").ToLocalChecked(),
      New(OBJECT_CALENDAR));
  Nan::Set(types, New("OBJECT_COMMAND").ToLocalChecked(),
      New(OBJECT_COMMAND));
  Nan::Set(types, New("OBJECT_DEVICE").ToLocalChecked(),
      New(OBJECT_DEVICE));
  Nan::Set(types, New("OBJECT_EVENT_ENROLLMENT").ToLocalChecked(),
      New(OBJECT_EVENT_ENROLLMENT));
  Nan::Set(types, New("OBJECT_FILE").ToLocalChecked(),
      New(OBJECT_FILE));
  Nan::Set(types, New("OBJECT_GROUP").ToLocalChecked(),
      New(OBJECT_GROUP));
  Nan::Set(types, New("OBJECT_LOOP").ToLocalChecked(),
      New(OBJECT_LOOP));
  Nan::Set(types, New("OBJECT_MULTI_STATE_INPUT").ToLocalChecked(),
      New(OBJECT_MULTI_STATE_INPUT));
  Nan::Set(types, New("OBJECT_MULTI_STATE_OUTPUT").ToLocalChecked(),
      New(OBJECT_MULTI_STATE_OUTPUT));
  Nan::Set(types, New("OBJECT_NOTIFICATION_CLASS").ToLocalChecked(),
      New(OBJECT_NOTIFICATION_CLASS));
  Nan::Set(types, New("OBJECT_PROGRAM").ToLocalChecked(),
      New(OBJECT_PROGRAM));
  Nan::Set(types, New("OBJECT_SCHEDULE").ToLocalChecked(),
      New(OBJECT_SCHEDULE));
  Nan::Set(types, New("OBJECT_AVERAGING").ToLocalChecked(),
      New(OBJECT_AVERAGING));
  Nan::Set(types, New("OBJECT_MULTI_STATE_VALUE").ToLocalChecked(),
      New(OBJECT_MULTI_STATE_VALUE));
  Nan::Set(types, New("OBJECT_TRENDLOG").ToLocalChecked(),
      New(OBJECT_TRENDLOG));
  Nan::Set(types, New("OBJECT_LIFE_SAFETY_POINT").ToLocalChecked(),
      New(OBJECT_LIFE_SAFETY_POINT));
  Nan::Set(types, New("OBJECT_LIFE_SAFETY_ZONE").ToLocalChecked(),
      New(OBJECT_LIFE_SAFETY_ZONE));
  Nan::Set(types, New("OBJECT_ACCUMULATOR").ToLocalChecked(),
      New(OBJECT_ACCUMULATOR));
  Nan::Set(types, New("OBJECT_PULSE_CONVERTER").ToLocalChecked(),
      New(OBJECT_PULSE_CONVERTER));
  Nan::Set(types, New("OBJECT_EVENT_LOG").ToLocalChecked(),
      New(OBJECT_EVENT_LOG));
  Nan::Set(types, New("OBJECT_GLOBAL_GROUP").ToLocalChecked(),
      New(OBJECT_GLOBAL_GROUP));
  Nan::Set(types, New("OBJECT_TREND_LOG_MULTIPLE").ToLocalChecked(),
      New(OBJECT_TREND_LOG_MULTIPLE));
  Nan::Set(types, New("OBJECT_LOAD_CONTROL").ToLocalChecked(),
      New(OBJECT_LOAD_CONTROL));
  Nan::Set(types, New("OBJECT_STRUCTURED_VIEW").ToLocalChecked(),
      New(OBJECT_STRUCTURED_VIEW));
  Nan::Set(types, New("OBJECT_ACCESS_DOOR").ToLocalChecked(),
      New(OBJECT_ACCESS_DOOR));
  Nan::Set(types, New("OBJECT_TIMER").ToLocalChecked(),
      New(OBJECT_TIMER));
  Nan::Set(types, New("OBJECT_ACCESS_CREDENTIAL").ToLocalChecked(),
      New(OBJECT_ACCESS_CREDENTIAL));
  Nan::Set(types, New("OBJECT_ACCESS_POINT").ToLocalChecked(),
      New(OBJECT_ACCESS_POINT));
  Nan::Set(types, New("OBJECT_ACCESS_RIGHTS").ToLocalChecked(),
      New(OBJECT_ACCESS_RIGHTS));
  Nan::Set(types, New("OBJECT_ACCESS_USER").ToLocalChecked(),
      New(OBJECT_ACCESS_USER));
  Nan::Set(types, New("OBJECT_ACCESS_ZONE").ToLocalChecked(),
      New(OBJECT_ACCESS_ZONE));
  Nan::Set(types, New("OBJECT_CREDENTIAL_DATA_INPUT").ToLocalChecked(),
      New(OBJECT_CREDENTIAL_DATA_INPUT));
  Nan::Set(types, New("OBJECT_NETWORK_SECURITY").ToLocalChecked(),
      New(OBJECT_NETWORK_SECURITY));
  Nan::Set(types, New("OBJECT_BITSTRING_VALUE").ToLocalChecked(),
      New(OBJECT_BITSTRING_VALUE));
  Nan::Set(types, New("OBJECT_CHARACTERSTRING_VALUE").ToLocalChecked(),
      New(OBJECT_CHARACTERSTRING_VALUE));
  Nan::Set(types, New("OBJECT_DATE_PATTERN_VALUE").ToLocalChecked(),
      New(OBJECT_DATE_PATTERN_VALUE));
  Nan::Set(types, New("OBJECT_DATE_VALUE").ToLocalChecked(),
      New(OBJECT_DATE_VALUE));
  Nan::Set(types, New("OBJECT_DATETIME_PATTERN_VALUE").ToLocalChecked(),
      New(OBJECT_DATETIME_PATTERN_VALUE));
  Nan::Set(types, New("OBJECT_DATETIME_VALUE").ToLocalChecked(),
      New(OBJECT_DATETIME_VALUE));
  Nan::Set(types, New("OBJECT_INTEGER_VALUE").ToLocalChecked(),
      New(OBJECT_INTEGER_VALUE));
  Nan::Set(types, New("OBJECT_LARGE_ANALOG_VALUE").ToLocalChecked(),
      New(OBJECT_LARGE_ANALOG_VALUE));
  Nan::Set(types, New("OBJECT_OCTETSTRING_VALUE").ToLocalChecked(),
      New(OBJECT_OCTETSTRING_VALUE));
  Nan::Set(types, New("OBJECT_POSITIVE_INTEGER_VALUE").ToLocalChecked(),
      New(OBJECT_POSITIVE_INTEGER_VALUE));
  Nan::Set(types, New("OBJECT_TIME_PATTERN_VALUE").ToLocalChecked(),
      New(OBJECT_TIME_PATTERN_VALUE));
  Nan::Set(types, New("OBJECT_TIME_VALUE").ToLocalChecked(),
      New(OBJECT_TIME_VALUE));
  Nan::Set(types, New("OBJECT_NOTIFICATION_FORWARDER").ToLocalChecked(),
      New(OBJECT_NOTIFICATION_FORWARDER));
  Nan::Set(types, New("OBJECT_ALERT_ENROLLMENT").ToLocalChecked(),
      New(OBJECT_ALERT_ENROLLMENT));
  Nan::Set(types, New("OBJECT_CHANNEL").ToLocalChecked(),
      New(OBJECT_CHANNEL));
  Nan::Set(types, New("OBJECT_LIGHTING_OUTPUT").ToLocalChecked(),
      New(OBJECT_LIGHTING_OUTPUT));
  Nan::Set(types, New("OBJECT_BINARY_LIGHTING_OUTPUT").ToLocalChecked(),
      New(OBJECT_BINARY_LIGHTING_OUTPUT));
  Nan::Set(types, New("OBJECT_NETWORK_PORT").ToLocalChecked(),
      New(OBJECT_NETWORK_PORT));
  Nan::Set(types, New("OBJECT_PROPRIETARY_MIN").ToLocalChecked(),
      New(OBJECT_PROPRIETARY_MIN));
  Nan::Set(types, New("OBJECT_PROPRIETARY_MAX").ToLocalChecked(),
      New(OBJECT_PROPRIETARY_MAX));
  Nan::Set(types, New("MAX_BACNET_OBJECT_TYPE").ToLocalChecked(),
      New(MAX_BACNET_OBJECT_TYPE));
  Nan::Set(target, New("BACNET_OBJECT_TYPE").ToLocalChecked(), types);
}

void setTagEnum(Local<Object> target) {
  Local<Object> tags = New<Object>();
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_NULL").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_NULL));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_BOOLEAN").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_BOOLEAN));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_UNSIGNED_INT").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_UNSIGNED_INT));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_SIGNED_INT").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_SIGNED_INT));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_REAL").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_REAL));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_DOUBLE").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_DOUBLE));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_OCTET_STRING").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_OCTET_STRING));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_CHARACTER_STRING").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_CHARACTER_STRING));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_BIT_STRING").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_BIT_STRING));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_ENUMERATED").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_ENUMERATED));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_DATE").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_DATE));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_TIME").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_TIME));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_OBJECT_ID").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_OBJECT_ID));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_RESERVE1").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_RESERVE1));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_RESERVE2").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_RESERVE2));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_RESERVE3").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_RESERVE3));
  Nan::Set(tags, New("MAX_BACNET_APPLICATION_TAG").ToLocalChecked(),
      New(MAX_BACNET_APPLICATION_TAG));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_EMPTYLIST").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_EMPTYLIST));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_WEEKNDAY").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_WEEKNDAY));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_DATERANGE").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_DATERANGE));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_DATETIME").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_DATETIME));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_TIMESTAMP").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_TIMESTAMP));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_ERROR").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_ERROR));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_DEVICE_OBJECT_PROPERTY_REFERENCE").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_DEVICE_OBJECT_PROPERTY_REFERENCE));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_DEVICE_OBJECT_REFERENCE").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_DEVICE_OBJECT_REFERENCE));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_OBJECT_PROPERTY_REFERENCE").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_OBJECT_PROPERTY_REFERENCE));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_DESTINATION").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_DESTINATION));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_RECIPIENT").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_RECIPIENT));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_COV_SUBSCRIPTION").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_COV_SUBSCRIPTION));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_CALENDAR_ENTRY").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_CALENDAR_ENTRY));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_WEEKLY_SCHEDULE").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_WEEKLY_SCHEDULE));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_SPECIAL_EVENT").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_SPECIAL_EVENT));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_READ_ACCESS_SPECIFICATION").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_READ_ACCESS_SPECIFICATION));
  Nan::Set(tags, New("BACNET_APPLICATION_TAG_LIGHTING_COMMAND").ToLocalChecked(),
      New(BACNET_APPLICATION_TAG_LIGHTING_COMMAND));
  Nan::Set(target, New("BACNET_APPLICATION_TAG").ToLocalChecked(), tags);
}
