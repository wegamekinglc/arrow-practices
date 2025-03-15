//
// Created by wegam on 2025/3/2.
//

#include <arrow/api.h>
#include <arrow/flight/api.h>
#include <iostream>

arrow::Status read_table_from_server(const std::string& work_dir)
{
    ARROW_ASSIGN_OR_RAISE(const auto location, arrow::flight::Location::ForGrpcTcp("localhost", 62001));
    ARROW_ASSIGN_OR_RAISE(const auto client, arrow::flight::FlightClient::Connect(location));
    std::cout << "Connected to " << location.ToString() << std::endl;

    auto descriptor = arrow::flight::FlightDescriptor::Path({work_dir + "/python/data/yellow_tripdata_2015-01.parquet"});
    ARROW_ASSIGN_OR_RAISE(auto flights_list, client->ListFlights());
    while (true) {
        ARROW_ASSIGN_OR_RAISE(auto flight_info, flights_list->Next());
        if (!flight_info) break;
        ARROW_ASSIGN_OR_RAISE(auto stream, client->DoGet(flight_info->endpoints()[0].ticket));
        ARROW_ASSIGN_OR_RAISE(auto table, stream->ToTable());
        arrow::PrettyPrintOptions print_options(/*indent=*/0, /*window=*/2);
        auto res = arrow::PrettyPrint(*table, print_options, &std::cout);
    }
    return arrow::Status::OK();
}

int main(int argc, char** argv) {
    if (argc <= 1)
        throw std::runtime_error("Usage: main arguments is missing");
    const std::string work_dir = argv[1];
    return static_cast<int>(read_table_from_server(work_dir).code());
}
