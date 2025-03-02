import sys
import pyarrow as pa
from pathlib import Path
from pyarrow.cffi import ffi

def run_export():

    ffi.cdef("""
        void export_int32_data(struct ArrowArray* array);
    """)
    if sys.platform == "win32":
        dll_path = str(Path(__file__).parents[1] / "cpp/lib/export_cdata.dll")
    else:
        dll_path = ""
    lib = ffi.dlopen(dll_path)
    # create a new pointer with ffi
    c_arr = ffi.new("struct ArrowArray*")
    # cast it to a uintptr_t
    c_ptr = int(ffi.cast("uintptr_t", c_arr))
    # call the function we made!
    lib.export_int32_data(c_arr)

    # import it via the C Data API so we can use it
    arr_new = pa.Array._import_from_c(c_ptr, pa.int32())
    # do stuff with the array like print it
    return arr_new

if __name__ == "__main__":
    print(run_export())
