import ctypes
import time


# define the timespec struct and clock_gettime function
class Timespec(ctypes.Structure):
    _fields_ = [("tv_sec", ctypes.c_long), ("tv_nsec", ctypes.c_long)]


clock_gettime = ctypes.CDLL("libc.so.6").clock_gettime
CLOCK_REALTIME = 0


def epoch():
    timespec = Timespec()
    if clock_gettime(CLOCK_REALTIME, ctypes.byref(timespec)) != 0:
        return 0.0

    # calculate time in seconds with fractional milliseconds
    return timespec.tv_sec + timespec.tv_nsec / 1e9


started_at = epoch()

total = 0
for i in range(1, 1000000001):
    total += i * i

taken = epoch() - started_at
print(f"Python took {taken:.6f} seconds to run the 1 billion benchmark")
print(total)
# Python took 47.040361 seconds to run the 1 billion benchmark
# 333333333833333333500000000
