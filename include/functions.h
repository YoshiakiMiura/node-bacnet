#ifndef NATIVE_EXTENSION_GRAB_H
#define NATIVE_EXTENSION_GRAB_H

#include <nan.h>
#include <v8.h>

NAN_METHOD(objectTypeToString);
NAN_METHOD(objectTypeToNumber);
NAN_METHOD(propertyKeyToString);
NAN_METHOD(propertyKeyToNumber);
NAN_METHOD(applicationTagToString);
NAN_METHOD(applicationTagToNumber);
NAN_METHOD(deviceStatusToString);
NAN_METHOD(deviceStatusToNumber);

NAN_METHOD(whois);
NAN_METHOD(isBound);
NAN_METHOD(readProperty);
NAN_METHOD(writeProperty);
NAN_METHOD(listen);
NAN_METHOD(initClient);
NAN_METHOD(initDevice);
NAN_METHOD(closeQueue);
NAN_METHOD(subscribeCov);
NAN_METHOD(timeSync);
NAN_METHOD(sendUCov);

#endif
