//
// Created by wegam on 2025/3/2.
//

#include <iostream>
#include <arrow/api.h>
#include <arrow/flight/api.h>

int main(int argc, char** argv) {
    arrow::flight::Location location = arrow::flight::Location::ForGrpcTcp("localhost", 62001).ValueOrDie();
    std::unique_ptr<arrow::flight::FlightClient> client = arrow::flight::FlightClient::Connect(location).ValueOrDie();
    std::cout << "Connected to " << location.ToString() << std::endl;

    auto descriptor = arrow::flight::FlightDescriptor::Path({"C:/Users/wegam/github/arrow-practices/python/data/yellow_tripdata_2015-01.parquet"});
    std::unique_ptr<arrow::flight::FlightListing> flights_list = client->ListFlights().ValueOrDie();
    while (true) {
        std::unique_ptr<arrow::flight::FlightInfo> flight_info = flights_list->Next().ValueOrDie();
        if (!flight_info) break;
        std::unique_ptr<arrow::flight::FlightStreamReader> stream = client->DoGet(flight_info->endpoints()[0].ticket).ValueOrDie();
        std::shared_ptr<arrow::Table> table = stream->ToTable().ValueOrDie();
        arrow::PrettyPrintOptions print_options(/*indent=*/0, /*window=*/2);
        auto res = arrow::PrettyPrint(*table, print_options, &std::cout);
    }
}
