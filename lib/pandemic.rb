
DELTA_X = 1.0
DELTA_Y = 1.0
DELTA_TIME = 0.1

alpha = 1.0
field = Field.new(100, 100)
field[5, 5] = 10.0

1000.times do |i|
	# field.print_grid
	
	# Heat equation
	field = field.map do |x, y|
		field[x, y] + DELTA_TIME * alpha * field.laplacian(x, y)
	end
	
	# sleep 0.2
	puts i
end

p field.values.inject(:+)