#include <stdio.h>
#include "wmbus_serializer.h"

int main(void)
{
    GatewayPayload g = {
        .gatewayId = "gateway_1234",
        .date = "1970-01-01",
        .deviceType = "stromleser",
        .interval_minutes = 15,
        .total_readings = 1,
        .values.device_count = 1
    };

    g.values.readings[0].media = "water";
    g.values.readings[0].meter = "waterstarm";
    g.values.readings[0].deviceId = "stromleser_50898527";
    g.values.readings[0].unit = "m3";
    g.values.readings[0].data_count = 1;

    g.values.readings[0].data[0] = (DataPoint){
        "1970-01-01 00:00", "1970-01-01 00:00", 107.752, "OK"
    };

    char buffer[2048];

    if (serialize_to_json(&g, buffer, sizeof(buffer)) == SERIALIZE_OK)
        printf("%s\n", buffer);
}
