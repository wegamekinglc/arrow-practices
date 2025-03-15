import pyarrow.compute as pc
import pyarrow.parquet as pq

table = pq.read_table("data/yellow_tripdata_2015-01.parquet")

incremented = pc.add(table["total_amount"], 5.5)
print(incremented)

res  = pc.min_max(table["total_amount"]).as_py()
print(f"min: {res['min']}, max: {res['max']}")

sorted = table.sort_by([("total_amount", "descending")])
print(sorted)
