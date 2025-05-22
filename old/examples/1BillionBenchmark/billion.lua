local ffi = require("ffi")

-- define the timespec struct and clock_gettime function
ffi.cdef [[
    typedef long time_t;

    struct timespec {
        time_t tv_sec;
        long tv_nsec;   // nanoseconds
    };

    int clock_gettime(int clk_id, struct timespec *tp);

    // clock id constants
    enum {
        CLOCK_REALTIME = 0
    };
]]

local function epoch()
    local timespec = ffi.new("struct timespec")
    local CLOCK_REALTIME = ffi.C.CLOCK_REALTIME

    if ffi.C.clock_gettime(CLOCK_REALTIME, timespec) ~= 0 then
        return 0
    end

    -- calculate time in sec with fractional milliseconds
    return tonumber(timespec.tv_sec) + tonumber(timespec.tv_nsec) / 1e9
end

local startedAt = epoch()

local total = 0
for i = 1, 1000000000 do
    total = total + i * i
end

local taken = epoch() - startedAt
print(string.format("Luvit Lua too %f seconds to run the 1 billion benchmark", taken))
print(total)
-- Luvit Lua too 0.353087 seconds to run the 1 billion benchmark
-- 3.3333333383336e+26
