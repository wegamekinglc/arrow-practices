import psutil
import os
import pyarrow as pa
import time

def get_memory():
    process = psutil.Process(os.getpid())

    while True:
        print(f"memory usage: {process.memory_info().rss / (1024 * 1024):.2f} MB - cpu usage: {process.cpu_percent(1):.2f}")
        # pa.system_memory_pool().release_unused()
        time.sleep(5)