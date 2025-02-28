import pyarrow as pa
import pyarrow.parquet as pq
import pyarrow.csv as pcsv

tbl = pq.read_table("data/yellow_tripdata_2015-01.parquet")
# pa.csv.write_csv(tbl, "data/yellow_tripdata_2015-01.csv")

# with pa.OSFile("data/yellow_tripdata_2015-01.arrow", "wb") as sink:
#     with pa.RecordBatchFileWriter(sink, tbl.schema) as writer:
#         writer.write_table(tbl)

df = tbl.to_pandas().fillna(0)
tbl = pa.Table.from_pandas(df)
with pa.OSFile("data/yellow_tripdata_2015-01-nonan.arrow", "wb") as sink:
    with pa.RecordBatchFileWriter(sink, tbl.schema) as writer:
        writer.write_table(tbl)
