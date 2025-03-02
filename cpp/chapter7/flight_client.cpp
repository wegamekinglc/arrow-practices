//
// Created by wegam on 2025/3/2.
//

#include <iostream>
#include <arrow/api.h>
#include <arrow/flight/api.h>

int main(int argc, char** argv) {
    arrow::flight::Location location;
    ARROW_ASSIGN_OR_RAISE(location,
                          arrow::flight::Location::ForGrpcTcp("localhost", 62001));
    std::unique_ptr<arrow::flight::FlightClient> client;
    ARROW_ASSIGN_OR_RAISE(client, arrow::flight::FlightClient::Connect(location));
    std::cout << "Connected to " << location.ToString() << std::endl;

    auto descriptor = arrow::flight::FlightDescriptor::Path({"C:/Users/wegam/github/arrow-practices/python/data/yellow_tripdata_2015-01.parquet"});

    std::unique_ptr<arrow::flight::FlightInfo> flight_info;
    ARROW_ASSIGN_OR_RAISE(flight_info, client->GetFlightInfo(descriptor));
    std::unique_ptr<arrow::flight::FlightStreamReader> stream;
    ARROW_ASSIGN_OR_RAISE(stream, client->DoGet(flight_info->endpoints()[0].ticket));
}
