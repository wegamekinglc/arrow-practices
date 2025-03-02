import pyarrow as pa
from pyarrow import flight

client = flight.connect(('localhost', 62001))
flights = list(client.list_flights())

for f in flights:
    print(f.descriptor.path, f.total_records)
    data: pa.Table = client.do_get(f.endpoints[0].ticket).read_all()
    print(data.shape)