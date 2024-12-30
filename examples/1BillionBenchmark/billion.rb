def epoch
  Time.now.to_f
end

start = epoch

total = 0
(1..1000000000).each do |i|
  total += i * i
end

taken = epoch - start

puts "Ruby took #{'%.9f' % taken} seconds to calculate the sum of squares from 1 to 1 billion:"
puts "Total: #{total}"