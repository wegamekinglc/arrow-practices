//
// Created by wegam on 2025/3/12.
//

#include <iostream>
#include <arrow/compute/api.h>
#include <arrow/io/api.h>
#include <arrow/table.h>
#include <parquet/arrow/reader.h>


arrow::Status read_table(const std::string& work_dir, std::shared_ptr<arrow::Table>* table)
{
    const auto filepath = work_dir + "/python/data/yellow_tripdata_2015-01.parquet";
    std::cout << "starting reading table: " << filepath << " ..." << std::endl;
    ARROW_ASSIGN_OR_RAISE(const auto input, arrow::io::ReadableFile::Open(filepath));
    ARROW_ASSIGN_OR_RAISE(const auto reader, parquet::arrow::OpenFile(input, arrow::default_memory_pool()));
    RETURN_NOT_OK(reader->ReadTable(table));
    std::cout << "end reading table ..." << std::endl;
    return arrow::Status::OK();
}


arrow::Status compute_parquet(const std::shared_ptr<arrow::Table>& table)
{
    const std::shared_ptr<arrow::ChunkedArray> column = table->GetColumnByName("total_amount");

    ARROW_ASSIGN_OR_RAISE(const auto incremented,
                          arrow::compute::CallFunction("add", {column, arrow::MakeScalar(5.5)}));
    // alternately we could do:
    ARROW_ASSIGN_OR_RAISE(const auto other_incremented, arrow::compute::Add(column, arrow::MakeScalar(5.5)));
    std::shared_ptr<arrow::ChunkedArray> output = std::move(incremented).chunked_array();
    std::cout << incremented.chunked_array()->ToString() << std::endl;
    std::cout << other_incremented.chunked_array()->ToString() << std::endl;
    return arrow::Status::OK();
}


arrow::Status find_minmax(const std::shared_ptr<arrow::Table>& table)
{
    const std::shared_ptr<arrow::ChunkedArray> column = table->GetColumnByName("total_amount");

    arrow::compute::ScalarAggregateOptions scalar_agg_opts;
    scalar_agg_opts.skip_nulls = false;
    ARROW_ASSIGN_OR_RAISE(const auto minmax, arrow::compute::MinMax(column, scalar_agg_opts));
    std::cout << minmax.scalar_as<arrow::StructScalar>().ToString() << std::endl;
    return arrow::Status::OK();
}


arrow::Status sort_table(const std::shared_ptr<arrow::Table>& table)
{
    arrow::compute::SortOptions sort_opts;
    sort_opts.sort_keys = {arrow::compute::SortKey{"total_amount", arrow::compute::SortOrder::Descending}};

    ARROW_ASSIGN_OR_RAISE(const auto indices, arrow::compute::SortIndices(table, sort_opts));
    ARROW_ASSIGN_OR_RAISE(const auto sorted, arrow::compute::Take(table, indices));
    std::cout << sorted.table()->ToString() << std::endl;
    return arrow::Status::OK();
}


int main(int argc, char** argv)
{
    if (argc <= 1)
        throw std::runtime_error("Usage: main arguments is missing");
    const std::string work_dir = argv[1];

    std::shared_ptr<arrow::Table> table;
    PARQUET_THROW_NOT_OK(read_table(work_dir, &table));

    PARQUET_THROW_NOT_OK(compute_parquet(table));
    PARQUET_THROW_NOT_OK(find_minmax(table));
    PARQUET_THROW_NOT_OK(sort_table(table));
}
