# PLEASE NOTE THAT gdate HAS BEEN USED AND THIS HAS ONLY BEEN TESTED ON macOS

# get start time in seconds with nanoseconds as fractional part
start=$(gdate +%s.%N)

./lexer.test FullTokenTests.nue

end=$(gdate +%s.%N)

elapsed=$(echo "$end - $start" | bc -l) # calculate difference in seconds
elapsed_ms=$(echo "$elapsed * 1000" | bc -l)
elapsed_ns=$(echo "$elapsed * 1000000000" | bc -l)

echo "Elapsed time: $elapsed_ms milliseconds, $elapsed_ns nanoseconds"
