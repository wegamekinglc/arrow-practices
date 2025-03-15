//
// Created by wegam on 2025/3/15.
//

#include <arrow/api.h>
#include <arrow/array/data.h>
#include <arrow/array/util.h>
#include <arrow/buffer.h>
#include <arrow/compute/api.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include "timer.hpp"

namespace cp = arrow::compute;

int main(int argc, char** argv) {
  for (int n = 10000; n <= 10000000; n += 1000000) {
    std::vector<int32_t> test_values(n);
    std::iota(std::begin(test_values), std::end(test_values), 0);

    arrow::Int32Builder num_bldr;
    ARROW_UNUSED(num_bldr.AppendValues(test_values));
    std::shared_ptr<arrow::Array> num_arr;
    ARROW_UNUSED(num_bldr.Finish(&num_arr));

    std::cout << "\nN: " << n << std::endl;


    arrow::Datum res1;
    {
      timer t;
      auto arr = std::static_pointer_cast<arrow::Int32Array>(num_arr);
      res1 = cp::Add(arr, arrow::Datum{(int32_t)2}).MoveValueUnsafe();
    }

    arrow::Datum res2;
    {
      timer t;
      auto arr = std::static_pointer_cast<arrow::Int32Array>(num_arr);
      arrow::Int32Builder bldr;
      for (size_t i = 0; i < arr->length(); ++i) {
        if (arr->IsValid(static_cast<int64_t>(i))) {
          ARROW_UNUSED(bldr.Append(arr->Value(i) + 2));
        } else {
          ARROW_UNUSED(bldr.AppendNull());
        }
      }
      std::shared_ptr<arrow::Array> output;
      ARROW_UNUSED(bldr.Finish(&output));
      res2 = arrow::Datum{output};
    }
    std::cout << std::boolalpha << (res1 == res2) << std::endl;

    arrow::Datum res3;
    {
      timer t;
      auto arr = std::static_pointer_cast<arrow::Int32Array>(num_arr);
      arrow::Int32Builder bldr;
      ARROW_UNUSED(bldr.Reserve(arr->length()));
      std::for_each(std::begin(*arr), std::end(*arr),
                    [&bldr](const auto& v) {
                      if (v) {
                        ARROW_UNUSED(bldr.Append(*v + 2));
                      } else {
                        ARROW_UNUSED(bldr.AppendNull());
                      }
                    });
      std::shared_ptr<arrow::Array> output;
      ARROW_UNUSED(bldr.Finish(&output));
      res3 = arrow::Datum{output};
    }
    std::cout << std::boolalpha << (res1 == res3) << std::endl;

    arrow::Datum res4;
    {
      timer t;
      auto arr = std::static_pointer_cast<arrow::Int32Array>(num_arr);
      std::shared_ptr<arrow::Buffer> new_buf =
          arrow::AllocateBuffer(static_cast<int64_t>(sizeof(int32_t)) * arr->length()).MoveValueUnsafe();
      auto output = reinterpret_cast<int32_t*>(new_buf->mutable_data());
      std::transform(arr->raw_values(), arr->raw_values() + arr->length(),
                     output, [](const int32_t v) { return v + 2; });

      res4 = arrow::Datum{arrow::MakeArray(
          arrow::ArrayData::Make(arr->type(),
                                 arr->length(),
                                 std::vector<std::shared_ptr<arrow::Buffer>>{arr->null_bitmap(), new_buf}, arr->null_count()))};
    }
    std::cout << std::boolalpha << (res1 == res4) << std::endl;
  }
}