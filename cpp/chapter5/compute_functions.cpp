//
// Created by wegam on 2025/3/12.
//

#include <iostream>
#include <arrow/compute/api.h>
#include <arrow/io/api.h>
#include <arrow/table.h>
#include <parquet/arrow/reader.h>

arrow::Status compute_parquet() {
    constexpr auto filepath = "C:/Users/wegam/github/arrow-practices/python/data/yellow_tripdata_2015-01.parquet";
    ARROW_ASSIGN_OR_RAISE(auto input, arrow::io::ReadableFile::Open(filepath));
    ARROW_ASSIGN_OR_RAISE(auto reader, parquet::arrow::OpenFile(input, arrow::default_memory_pool()));

    std::shared_ptr<arrow::Table> table;
    RETURN_NOT_OK(reader->ReadTable(&table));
    const std::shared_ptr<arrow::ChunkedArray> column = table->GetColumnByName("total_amount");
    std::cout << column->ToString() << std::endl;

    ARROW_ASSIGN_OR_RAISE(auto incremented, arrow::compute::Add(column, arrow::MakeScalar(5.5)));
    // std::shared_ptr<arrow::Scalar> increment = arrow::MakeScalar(5.5);
    // arrow::Datum incremented = arrow::compute::CallFunction("add", {column, increment}).ValueOrDie();
    // ARROW_ASSIGN_OR_RAISE(incremented, arrow::compute::CallFunction("add", {column, increment}));
    // alternately we could do:
    // ARROW_ASSIGN_OR_RAISE(auto other_incremented,
    //                       arrow::compute::Add(column, arrow::MakeScalar(5.5)));
    // std::shared_ptr<arrow::ChunkedArray> output =
    //     std::move(incremented).chunked_array();
    // std::cout << output->ToString() << std::endl;
    // std::cout << other_incremented.chunked_array()->ToString() << std::endl;
    return arrow::Status::OK();
}

int main(int argc, char** argv) {
    PARQUET_THROW_NOT_OK(compute_parquet());
}
