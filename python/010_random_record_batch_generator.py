import pyarrow as pa
import numpy as np

N_ROWS = 8192
N_COLS = 16

data = [pa.array(np.random.randn(N_ROWS)) for i in range(N_COLS)]
cols = ["C" + str(i) for i in range(N_COLS)]
rb = pa.RecordBatch.from_arrays(data, cols)

print(rb.schema)
print(rb.num_rows)

archer_list =[
    {
        "archer": "Legolas",
        "location": "Murkwood",
        "year": 1954
    },
    {
        "archer": "Oliver",
        "location": "Star City",
        "year": 1941,
    },
    {
        "archer": "Merida",
        "location": "Scotland",
        "year": 2012,
    },
    {
        "archer": "Lara",
        "location": "London",
        "year": 1996,
    },
    {
        "archer": "Artemis",
        "location": "Greece",
        "year": -600,
    }
]

archer_type = pa.struct([("archer", pa.utf8()),
                         ("location", pa.utf8()),
                         ("year", pa.int16())])
archers = pa.array(archer_list, type=archer_type)
print(archers.type)
print(archers)

rb = pa.RecordBatch.from_arrays(archers.flatten(),
                                ["archer", "location", "year"])
print(rb)
print(rb.num_rows)
print(rb.num_columns)

slice = rb.slice(1, 3)
print(slice.num_rows)
print(rb.column(0)[0])
print(slice.column(0)[0])

archers_slice = archers[1:3]
print(rb.to_pydict())
print(archers.to_pylist())

from typing import NamedTuple, List


class DataRow(NamedTuple):

    id: int
    component: int
    component_cost: List[float]


def vector_to_columnar(rows: List[DataRow]):
    return pa.table(
        {
            "id": [r.id for r in rows],
            "component": [r.component for r in rows],
            "component_cost": [r.component_cost for r in rows]
        }
    )


def columnar_to_vector(tbl: pa.Table):
    ids = tbl.column(0).to_pylist()
    component = tbl.column(1).to_pylist()
    component_cost = tbl.column(2).to_pylist()

    out = []
    for i in range(tbl.num_rows):
        out.append(
            DataRow(ids[i], component[i], component_cost[i])
        )
    return out


orig = [
    DataRow(1, 1, [10]),
    DataRow(2, 3, [11,12, 13]),
    DataRow(3, 2, [15, 25]),
]
tbl = vector_to_columnar(orig)
converted = columnar_to_vector(tbl)
assert len(orig) == len(converted)

print(converted)