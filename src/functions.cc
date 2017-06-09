// for addon
#include <node.h>
#include <v8.h>
#include <nan.h>
#include <string>
#include <iostream>
#include <sstream>
#include "functions.h"
#include "listen.h"
#include "init.h"
#include "listenable.h"
#include "client.h"
#include "newclient.h"
#include "bactext.h"
#include "address.h"
#include "conversion.h"
#include "BacnetValue.h"
#include "device.h"
#include "dlenv.h"
#include "datalink.h"


// Returns the object type string for an object type number. If a string is provided, it is checked for validity and returned
NAN_METHOD(objectTypeToString) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1 && info[0]->IsString()) {
        unsigned index;
        if (bactext_object_type_index(extractString(info[0].As<v8::String>()).c_str(), &index)) {
            const char * name = bactext_object_type_name(index);
            info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
        } else {
            errorStringStream << "Object type string not valid" << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
        }
    } else if (info.Length() >= 1 && info[0]->IsUint32()) {
        const char * name = bactext_object_type_name(info[0]->Uint32Value());
        info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
    } else {
        errorStringStream << "Object type must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

// Returns the object type number for an object type string. If a number is provided it is checked for validity and returned
NAN_METHOD(objectTypeToNumber) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1) {
        unsigned value;
        const char * error = objectTypeToC(info[0], &value);
        if (error) {
            errorStringStream << "Object type string not valid : " << error << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
        } else {
            info.GetReturnValue().Set(Nan::New(value));
        }
    } else {
        errorStringStream << "Object type must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

// Returns the property key string for the property number provided. If a string is provided it is checked for validity and returned
NAN_METHOD(propertyKeyToString) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1 && info[0]->IsString()) {
        unsigned index;
        if (bactext_property_index(inputString.c_str(), &index)) {
            const char * name = bactext_property_name(index);
            info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
        } else {
            errorStringStream << "Property key string not valid" << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
         }
    } else if (info.Length() >= 1 && info[0]->IsUint32()) {
        uint32_t propertyKey = info[0]->Uint32Value();
        if (propertyKey <= MAX_BACNET_PROPERTY_ID) {
            const char * name = bactext_property_name(propertyKey);
            info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
        } else {
            errorStringStream << "Property key too large, maximum is 4194303" << ", provided : " << inputString;
            Nan::ThrowRangeError(errorStringStream.str().c_str());
        }
    } else {
        errorStringStream << "Property key must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

// Returns the property number for a property key string provided. If a number is provided it is checked for validity and returned
NAN_METHOD(propertyKeyToNumber) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1 && info[0]->IsString()) {
        unsigned index;
        if (bactext_property_index(inputString.c_str(), &index)) {
            info.GetReturnValue().Set(Nan::New(index));
        } else {
            errorStringStream << "Property key string not valid" << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
        }
    } else if (info.Length() >= 1 && info[0]->IsUint32()) {
        uint32_t propertyKey = info[0]->Uint32Value();
        if (propertyKey <= MAX_BACNET_PROPERTY_ID) {
            info.GetReturnValue().Set(Nan::New(propertyKey));
        } else {
            errorStringStream << "Property key too large, maximum is 4194303" << ", provided : " << inputString;
            Nan::ThrowRangeError(errorStringStream.str().c_str());
        }
    } else {
        errorStringStream << "Property key must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

// Returns the application tag string for the application tag number provided. If a string is provided it is checked for validity and returned
NAN_METHOD(applicationTagToString) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1 && info[0]->IsString()) {
        unsigned index;
        if (bactext_application_tag_index(inputString.c_str(), &index)) {
            const char * name = bactext_application_tag_name(index);
            info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
        } else {
            errorStringStream << "Application tag string not valid" << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
         }
    } else if (info.Length() >= 1 && info[0]->IsUint32()) {
        uint32_t applicationTag = info[0]->Uint32Value();
        if (applicationTag <= MAX_BACNET_APPLICATION_TAG) {
            const char * name = bactext_application_tag_name(applicationTag);
            info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
        } else {
            errorStringStream << "Application tag too large, maximum is " << MAX_BACNET_APPLICATION_TAG << ", provided : " << inputString;
            Nan::ThrowRangeError(errorStringStream.str().c_str());
        }
    } else {
        errorStringStream << "Application tag must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

// Returns the application tag number for a application tag string provided. If a number is provided it is checked for validity and returned
NAN_METHOD(applicationTagToNumber) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1 && info[0]->IsString()) {
        unsigned index;
        if (bactext_application_tag_index(inputString.c_str(), &index)) {
            info.GetReturnValue().Set(Nan::New(index));
        } else {
            errorStringStream << "Application tag string not valid" << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
        }
    } else if (info.Length() >= 1 && info[0]->IsUint32()) {
        uint32_t applicationTag = info[0]->Uint32Value();
        if (applicationTag <= MAX_BACNET_APPLICATION_TAG) {
            info.GetReturnValue().Set(Nan::New(applicationTag));
        } else {
            errorStringStream << "Application tag too large, maximum is " << MAX_BACNET_APPLICATION_TAG << ", provided : " << inputString;
            Nan::ThrowRangeError(errorStringStream.str().c_str());
        }
    } else {
        errorStringStream << "Application tag must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

NAN_METHOD(deviceStatusToString) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1 && info[0]->IsString()) {
        unsigned index;
        if (bactext_device_status_index(inputString.c_str(), &index)) {
            const char * name = bactext_device_status_name(index);
            info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
        } else {
            errorStringStream << "Device status string not valid" << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
         }
    } else if (info.Length() >= 1 && info[0]->IsUint32()) {
        uint32_t deviceStatus = info[0]->Uint32Value();
        if (deviceStatus <= MAX_DEVICE_STATUS) {
            const char * name = bactext_device_status_name(deviceStatus);
            info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());
        } else {
            errorStringStream << "Device status too large, maximum is " << MAX_DEVICE_STATUS << ", provided : " << inputString;
            Nan::ThrowRangeError(errorStringStream.str().c_str());
        }
    } else {
        errorStringStream << "Device status must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

NAN_METHOD(deviceStatusToNumber) {
    std::ostringstream errorStringStream;
    std::string inputString = extractString(info[0].As<v8::String>());
    if (info.Length() >= 1 && info[0]->IsString()) {
        unsigned index;
        if (bactext_device_status_index(inputString.c_str(), &index)) {
            info.GetReturnValue().Set(Nan::New(index));
        } else {
            errorStringStream << "Device status string not valid" << ", provided : " << inputString;
            Nan::ThrowError(errorStringStream.str().c_str());
        }
    } else if (info.Length() >= 1 && info[0]->IsUint32()) {
        uint32_t deviceStatus = info[0]->Uint32Value();
        if (deviceStatus <= MAX_DEVICE_STATUS) {
            info.GetReturnValue().Set(Nan::New(deviceStatus));
        } else {
            errorStringStream << "Device status too large, maximum is " << MAX_BACNET_APPLICATION_TAG << ", provided : " << inputString;
            Nan::ThrowRangeError(errorStringStream.str().c_str());
        }
    } else {
        errorStringStream << "Device status must be either a string or unsigned int" << ", provided : " << inputString;
        Nan::ThrowError(errorStringStream.str().c_str());
    }
}

// whois([destination, [min_id , [max_id]]])
// Send a whois request to the destination device, optionally specifying a minimum and maximum device id
NAN_METHOD(whois) {
    int32_t min_id = -1;
    int32_t max_id = -1;
    BACNET_ADDRESS dest = bacnetAddressToC(info[0]);

    if (info.Length() > 1) {
        min_id = max_id = info[1]->Int32Value();
    }
    if (info.Length() > 2) {
        max_id = info[2]->Int32Value();
    }

    if (min_id > BACNET_MAX_INSTANCE) {
        fprintf(stderr, "device-instance-min=%u - it must be less than %u\n",
            min_id, BACNET_MAX_INSTANCE + 1);
        return;
    }
    if (max_id > BACNET_MAX_INSTANCE) {
        fprintf(stderr, "device-instance-max=%u - it must be less than %u\n",
            max_id, BACNET_MAX_INSTANCE + 1);
        return;
    }
    /* send the request */
    Send_WhoIs_To_Network(&dest, min_id,
        max_id);

    info.GetReturnValue().Set(Nan::New(true));
}

// Get a bacnet address, either by converting a representation of an address, or, if the value is a number, looking it
// up as a device id in the table of devices bound to addresses
bool addressOrBoundDeviceIdToC(Local<Value> value, unsigned * max_apdu, BACNET_ADDRESS * dest) {
    if (value->IsNumber()) {  // device id
        int32_t device_id = value->Int32Value();

        /* is the device bound? */
        bool isBound = address_get_by_device(device_id, max_apdu, dest);
        if (!isBound) {
            Nan::ThrowError("device is not bound\n");
            return false;
        }
    } else {   // device address
        *dest = bacnetAddressToC(value);
        *max_apdu = MAX_APDU; // without doing the whois we dont know the Max apdu for the device - so we will just hope it is the same as ours
    }
    return true;
}

// readProperty(deviceId, objectType, objectId, property [, arrayIndex])
// Read a property of a remote device
NAN_METHOD(readProperty) {
    BACNET_ADDRESS dest = {};
    unsigned max_apdu = 0;

    bool addressed = addressOrBoundDeviceIdToC(info[0], &max_apdu, &dest);
    int32_t object_type = info[1]->Int32Value();
    int32_t object_instance = info[2]->Int32Value();
    int32_t object_property = info[3]->Int32Value();
    uint32_t array_index = BACNET_ARRAY_ALL;

    if (info[4]->IsUint32()) {
        array_index = info[4]->Uint32Value();
    }

    if (addressed) {
        int invoke_id = Send_Read_Property_Request_Address(
            &dest,
            max_apdu,
            (BACNET_OBJECT_TYPE)object_type,
            object_instance,
            (BACNET_PROPERTY_ID)object_property,
            array_index);

        info.GetReturnValue().Set(Nan::New(invoke_id));
    } else {
        Nan::ThrowError("Unable to resolve address for read.");
    }
}

// isBound(deviceId)
// checks whether a deviceId is already bound and can be used to address a device
NAN_METHOD(isBound) {
    BACNET_ADDRESS dest = {};
    unsigned max_apdu = 0;

    int32_t device_id = info[0]->Int32Value();
    bool bound = address_get_by_device(device_id, &max_apdu, &dest);
    info.GetReturnValue().Set(Nan::New(bound));
}

// writeProperty(deviceId, objectType, objectId, property, arrayIndex, value [, priority])
// Write a property to a remote device
NAN_METHOD(writeProperty) {
    BACNET_ADDRESS dest = {};
    unsigned max_apdu = 0;

    bool addressed = addressOrBoundDeviceIdToC(info[0], &max_apdu, &dest);
    int32_t object_type = info[1]->Int32Value();
    int32_t object_instance = info[2]->Int32Value();
    int32_t object_property = info[3]->Int32Value();
    uint32_t array_index = BACNET_ARRAY_ALL;
    if (info[4]->IsUint32()) {
        array_index = info[4]->Uint32Value();
    }
    Local<Object> valueObject = Nan::To<Object>(info[5]).ToLocalChecked();
    uint8_t priority = BACNET_NO_PRIORITY;
    if (info[6]->IsUint32()) {
        priority = info[6]->Uint32Value();
    }

    if (addressed) {
        BacnetValue * bacnetValue = BacnetValue::Unwrap<BacnetValue>(valueObject);
        BACNET_APPLICATION_DATA_VALUE object_value = {};
        if (bacnetValue->bacnetValue(&object_value)) {
            int invoke_id = Send_Write_Property_Request_Address(
                &dest,
                max_apdu,
                (BACNET_OBJECT_TYPE)object_type,
                object_instance,
                (BACNET_PROPERTY_ID)object_property,
                &object_value,
                priority,
                array_index);
            info.GetReturnValue().Set(Nan::New(invoke_id));
        } else {
            Nan::ThrowError("Some kind of error converting bacnet value for write property");
        }
    } else {
        Nan::ThrowError("Unable to resolve address for write.");
    }
}

// Start a separate thread to listen to the bacnet socket and run the layers of the bacnet stack
NAN_METHOD(listen) {
  listenLoop();
}

NAN_METHOD(initClient) {
  v8::Local<v8::Object> localEventEmitter = info[0]->ToObject();
  eventEmitterSet(localEventEmitter);

  init_service_handlers();
}

NAN_METHOD(initDevice) {
  init_device_service_handlers();
}

NAN_METHOD(closeQueue) {
  eventEmitterClose();
}

// subscribeCov(deviceId, objectType, objectId, pid, type])
NAN_METHOD(subscribeCov) {

  BACNET_ADDRESS dest = {};
  unsigned max_apdu = 0;

  bool addressed = addressOrBoundDeviceIdToC(info[0], &max_apdu, &dest);
  int16_t object_type = info[1]->Int32Value();
  int32_t object_instance = info[2]->Int32Value();
  uint32_t pid = info[3]->Uint32Value();
  std::string type = extractString(info[4].As<v8::String>());

  if (addressed) {
    BACNET_SUBSCRIBE_COV_DATA cov_data = {};
    cov_data.monitoredObjectIdentifier.type = object_type;
    cov_data.monitoredObjectIdentifier.instance = object_instance;
    cov_data.subscriberProcessIdentifier = pid;
    cov_data.lifetime = 0;
    // confirmed or unconfirmed
    cov_data.issueConfirmedNotifications = type == "confirmed" ;
    int invoke_id = Send_COV_Subscribe_Address(&dest, max_apdu, &cov_data);

    info.GetReturnValue().Set(Nan::New(invoke_id));

  } else {
    Nan::ThrowError("Unable to resolve address for subscribe.");
  }
}

// timeSync(deviceId, objectType, objectId, pid, type])
NAN_METHOD(timeSync) {
  BACNET_ADDRESS dest = {};
  unsigned max_apdu = 0;
  bool addressed = addressOrBoundDeviceIdToC(info[0], &max_apdu, &dest);
  time_t rawtime;
  struct tm *my_time;
  BACNET_DATE bdate;
  BACNET_TIME btime;

  if (addressed) {
    time(&rawtime);
    my_time = localtime(&rawtime);
    bdate.year = my_time->tm_year + 1900;
    bdate.month = my_time->tm_mon + 1;
    bdate.day = my_time->tm_mday;
    bdate.wday = my_time->tm_wday ? my_time->tm_wday : 7;
    btime.hour = my_time->tm_hour;
    btime.min = my_time->tm_min;
    btime.sec = my_time->tm_sec;
    btime.hundredths = 0;

    Send_TimeSync_Remote(&dest, &bdate, &btime);
  } else {
    Nan::ThrowError("Unable to resolve address for time sync.");
  }
}

// sendUCov(pid, deviceId, objectType, objectId, time, propertyId, value)
// Write a property to a remote device
NAN_METHOD(sendUCov) {

  BACNET_ADDRESS dest = {};
  BACNET_COV_DATA cov_data;
  BACNET_PROPERTY_VALUE value_list = {};
  unsigned max_apdu = 0;

  bool addressed = addressOrBoundDeviceIdToC(info[1], &max_apdu, &dest);

  if (addressed) {

    uint32_t pid = info[0]->Int32Value();
    uint32_t initiatingDeviceIdentifier = info[1]->Int32Value();
    int32_t object_type = info[2]->Int32Value();
    int32_t object_instance = info[3]->Int32Value();
    int32_t time = info[4]->Int32Value();
    int32_t object_property = info[5]->Int32Value();
    Local<Object> valueObject = Nan::To<Object>(info[6]).ToLocalChecked();

    BacnetValue * bacnetValue = BacnetValue::Unwrap<BacnetValue>(valueObject);
    BACNET_APPLICATION_DATA_VALUE bAppDataVal = {};
    if (bacnetValue->bacnetValue(&bAppDataVal)) {
      value_list.propertyIdentifier = (BACNET_PROPERTY_ID) object_property;
      value_list.value = bAppDataVal;
      value_list.next = NULL;
    } else {
      Nan::ThrowError("Invalid application data value.");
    }


    cov_data.subscriberProcessIdentifier = pid;
    cov_data.initiatingDeviceIdentifier = initiatingDeviceIdentifier;
    cov_data.monitoredObjectIdentifier.type = object_type;
    cov_data.monitoredObjectIdentifier.instance = object_instance;
    cov_data.timeRemaining = time;
    cov_data.listOfValues = &value_list;

    /** @file txbuf.c  Declare the global Transmit Buffer for handler functions. */

    uint8_t Handler_Transmit_Buffer[MAX_PDU] = { 0 };

    Send_UCOV_Notify(&Handler_Transmit_Buffer[0], &cov_data);

  } else {
    Nan::ThrowError("Unable to resolve address.");
  }
}
