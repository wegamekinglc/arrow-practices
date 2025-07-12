import pyarrow as pa
import numpy as np
import pyarrow.parquet as pq


def generate(n):
    sid_samples = [f"sid_{i}" for i in range(1, 1001)]
    ind_samples = [f"ins_{i}" for i in range(1, 2001)]

    bool_samples = [True, False]
    sids = np.random.choice(sid_samples, n)
    instruments = np.random.choice(ind_samples, n)
    d_values = np.random.randn(3, n)
    b_values = np.random.choice(bool_samples, n)

    schema = pa.schema([pa.field("StrategyID", pa.string()),
                        pa.field("InstrumentID", pa.string()),
                        pa.field("IndCode1", pa.float64()),
                        pa.field("IndCode2", pa.float64()),
                        pa.field("IndCode3", pa.float64()),
                        pa.field("Flag", pa.bool_())])
    df = pa.Table.from_arrays([sids, instruments, d_values[0], d_values[1], d_values[2], b_values], schema=schema)
    pq.write_table(df, "sample.parquet")
    print(f"finished generating data with shape: {df.shape}")


if __name__ == "__main__":
    import sys
    n = int(sys.argv[1])
    generate(n)