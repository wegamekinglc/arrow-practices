from pathlib import Path
import pyarrow.parquet as pq
from pyarrow import fs
import pyarrow.flight as flight


class Server(flight.FlightServerBase):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        print("Initializing Flight Server")
        self._lc = fs.LocalFileSystem()

    def list_flights(self, context, criteria):
        path = str(Path(__file__).parent / "data")
        if len(criteria) > 0:
            path += "/" + criteria.decode("utf-8")
        f_list = self._lc.get_file_info(fs.FileSelector(path, recursive=True))
        for f_info in f_list:
            f_path  = f_info.path
            if f_info.type == fs.FileType.Directory:
                continue

            if not f_path.endswith(".parquet"):
                continue
            print("Found flight file: {}".format(f_path))
            with self._lc.open_input_file(f_path) as f:
                data = pq.ParquetFile(f, pre_buffer=True)
                yield flight.FlightInfo(
                    data.schema_arrow,
                    flight.FlightDescriptor.for_path(f_path),
                    [flight.FlightEndpoint(f_path, [])],
                    data.metadata.num_rows,
                    -1
                )

    def do_get(self, context, ticket):
        input_file = self._lc.open_input_file(ticket.ticket.decode("utf-8"))
        print("Getting flight data for ticket {}".format(input_file))
        pf = pq.ParquetFile(input_file, pre_buffer=True)
        def gen():
            try:
                for batch in pf.iter_batches():
                    yield batch
            finally:
                input_file.close()
        return flight.GeneratorStream(pf.schema_arrow, gen())


if __name__ == "__main__":
    server = Server(location=("localhost", 64648))
    print(f"Starting Flight Server at port: {server.port}")
    server.serve()