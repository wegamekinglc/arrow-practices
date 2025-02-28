import gc
import os
import datetime as dt
import psutil
import pyarrow as pa
import pyarrow.csv as pcsv
import pyarrow.compute as pc
import pandas as pd
import pyarrow.parquet as pq

gc.disable()

memory_init = psutil.Process(os.getpid()).memory_info().rss >> 20
time_init = dt.datetime.now()

col_pd_csv = pd.read_csv("data/yellow_tripdata_2015-01.csv", usecols=["total_amount"])["total_amount"]
col_pd_csv.mean()
memory_pd_csv = psutil.Process(os.getpid()).memory_info().rss >> 20
time_pd_csv = dt.datetime.now()

col_pa_csv = pcsv.read_csv("data/yellow_tripdata_2015-01.csv",
                             convert_options=pa.csv.ConvertOptions(include_columns=["total_amount"]))
pc.mean(col_pa_csv["total_amount"])
memory_pa_csv = psutil.Process(os.getpid()).memory_info().rss >> 20
time_pa_csv = dt.datetime.now()

col_parquet = pq.read_table("data/yellow_tripdata_2015-01.parquet", columns=["total_amount"])
pc.mean(col_parquet["total_amount"])
memory_parquet = psutil.Process(os.getpid()).memory_info().rss >> 20
time_parquet = dt.datetime.now()

with pa.OSFile("data/yellow_tripdata_2015-01.arrow", "rb") as source:
    col_arrow_file = pa.ipc.open_file(source).read_all().column("total_amount")
pc.mean(col_arrow_file)
memory_arrow = psutil.Process(os.getpid()).memory_info().rss >> 20
time_arrow = dt.datetime.now()

source = pa.memory_map("data/yellow_tripdata_2015-01.arrow", "rb")
col_arrow_mmap = pa.ipc.RecordBatchFileReader(source).read_all().column("total_amount")
pc.mean(col_arrow_mmap)
memory_mmap = psutil.Process(os.getpid()).memory_info().rss >> 20
time_mmap = dt.datetime.now()

print("pandas: ", memory_pd_csv - memory_init, " MB - ", time_pd_csv - time_init)
print("pyarrow: ", memory_pa_csv - memory_pd_csv, " MB - ", time_pa_csv - time_pd_csv)
print("parquet col: ", memory_parquet - memory_pa_csv, " MB - ", time_parquet - time_pa_csv)
print("arrow ipc: ", memory_arrow - memory_parquet, " MB - ", time_arrow - time_parquet)
print("mmap zero-copy: ", memory_mmap - memory_arrow, " MB - ", time_mmap - time_arrow)