import pathlib
import pyarrow.flight as flight
import pyarrow.parquet as pq
from utils import get_memory
import threading


class FlightServer(flight.FlightServerBase):
    def __init__(self, location="grpc://0.0.0.0:8816", **kwargs):
        super().__init__(location, **kwargs)
        self._repo = pathlib.Path("./")
        self._location = location
        t = threading.Thread(target=get_memory, daemon=True)
        t.start()
        print("init finished")

    def _make_flight_info(self, dataset):
        dataset_path = self._repo / dataset
        schema = pq.read_schema(dataset_path)
        metadata = pq.read_metadata(dataset_path)
        descriptor = flight.FlightDescriptor.for_path(dataset.encode("utf8"))
        endpoints = [flight.FlightEndpoint(dataset, [self._location])]
        return flight.FlightInfo(
            schema=schema,
            descriptor=descriptor,
            endpoints=endpoints,
            total_records=metadata.num_rows,
            total_bytes=metadata.serialized_size
        )
    
    def list_flights(self, context, criteria):
        for dataset in self._repo.iterdir():
            if dataset.is_file() and dataset.suffix == ".parquet":
                yield self._make_flight_info(dataset.name)

    def get_flight_info(self, context, descriptor):
        dataset = descriptor.path[0].decode("utf8")
        if not (self._repo / dataset).exists():
            raise flight.FlightNotFound(f"dataset {dataset} not found")
        return self._make_flight_info(dataset)
    
    def do_get(self, context, ticket):
        dataset = ticket.ticket.decode("utf8")
        dataset_path = self._repo / dataset
        if not dataset_path.exists():
            raise flight.FlightNotFound(f"dataset {dataset} not found")

        pf = pq.ParquetFile(dataset_path)
        return flight.GeneratorStream(pf.schema_arrow, pf.iter_batches(batch_size=1_000_000))


if __name__ == "__main__":
    server = FlightServer()
    server.serve()
       