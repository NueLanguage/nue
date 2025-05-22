# 1 Billion Benchmark

The 1 billion benchmark results provided in the comments of each file were produced on the following hardware:

Intel Core i9-14900KF

Please note that different hardware may yield different results in terms of time taken to run the benchmark.
It is advised that you run the benchmark yourself on your own hardware to provide yourself with a relevant and understandable baseline.

## Results

TODO: clarify that "Valid Result" means equating to the actual expected number (333333...etc)
TODO: re-run all of the tests outside of vscode because vscode shell uses a lot of resources for some odd reason

| Programming Language | Optimisations | Time (sec) | Result | Valid Result |
|-|-|-|-|-|
| Lua (Luvit) | LuaJIT | `0.353087` | `3.3333333383336e+26` | ✅ |
| Python |   | `47.040361` | `333333333833333333500000000` | ✅ |
| JavaScript | nodeJS | `77.65400004386902` | `333333333833333333500000000` | ✅ |
| TypeScript | ts-node | `70.665999889` | `333333333833333333349161825` | ✅ |
| Ruby |   | `108.687818289` | `333333333833333333500000000` | ✅ |
