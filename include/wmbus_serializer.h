#ifndef WMBUS_SERIALIZER_H
#define WMBUS_SERIALIZER_H

#include <stddef.h>

#define MAX_DEVICES 4
#define MAX_POINTS  8

typedef struct {
    const char* timestamp;
    const char* meter_datetime;
    double total_value;
    const char* status;
} DataPoint;

typedef struct {
    const char* media;
    const char* meter;
    const char* deviceId;
    const char* unit;
    int data_count;
    DataPoint data[MAX_POINTS];
} DeviceReading;

typedef struct {
    int device_count;
    DeviceReading readings[MAX_DEVICES];
} MeasurementBlock;

typedef struct {
    const char* gatewayId;
    const char* date;
    const char* deviceType;
    int interval_minutes;
    int total_readings;
    MeasurementBlock values;
} GatewayPayload;

typedef enum {
    SERIALIZE_OK = 0,
    SERIALIZE_BUFFER_TOO_SMALL,
    SERIALIZE_INVALID_INPUT
} SerializeStatus;

SerializeStatus serialize_to_json(
        const GatewayPayload* input,
        char* output,
        size_t buffer_size);

size_t max_json_size(void);

#endif
