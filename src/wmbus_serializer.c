#include "wmbus_serializer.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


static size_t safe_append(char* buf, size_t max, size_t pos, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(buf + pos, max - pos, fmt, args);
    va_end(args);

    if (written < 0 || pos + written >= max)
        return max;

    return pos + written;
}

SerializeStatus serialize_to_json(const GatewayPayload* in, char* out, size_t max)
{
    if (!in || !out) return SERIALIZE_INVALID_INPUT;

    size_t pos = 0;

    pos = safe_append(out, max, pos,
        "[{\"gatewayId\":\"%s\",\"date\":\"%s\",\"deviceType\":\"%s\","
        "\"interval_minutes\":%d,\"total_readings\":%d,\"values\":{"
        "\"device_count\":%d,\"readings\":[",
        in->gatewayId, in->date, in->deviceType,
        in->interval_minutes, in->total_readings,
        in->values.device_count);

    for (int i = 0; i < in->values.device_count; ++i) {
    	const DeviceReading* d = &in->values.readings[i];


        pos = safe_append(out, max, pos,
            "{\"media\":\"%s\",\"meter\":\"%s\",\"deviceId\":\"%s\",\"unit\":\"%s\",\"data\":[",
            d->media, d->meter, d->deviceId, d->unit);

        for (int j = 0; j < d->data_count; ++j) {
        	const DataPoint* p = &d->data[j];


            pos = safe_append(out, max, pos,
                "{\"timestamp\":\"%s\",\"meter_datetime\":\"%s\",\"total_m3\":%.3f,\"status\":\"%s\"}%s",
                p->timestamp, p->meter_datetime, p->total_value, p->status,
                (j + 1 < d->data_count) ? "," : "");
        }

        pos = safe_append(out, max, pos, "]}%s", (i + 1 < in->values.device_count) ? "," : "");
    }

    pos = safe_append(out, max, pos, "]}}]");

    if (pos >= max) return SERIALIZE_BUFFER_TOO_SMALL;

    return SERIALIZE_OK;
}

size_t max_json_size(void)
{
    return 2048;
}
