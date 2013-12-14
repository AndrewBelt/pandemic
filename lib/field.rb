
class Field
	attr_reader :width
	attr_reader :height
	attr_accessor :values
	
	def initialize(width, height)
		@width = width
		@height = height
		@values = Array.new(@width * @height, 0.0)
	end
	
	def map
		other = Field.new(@width, @height)
		
		@height.times do |y|
			@width.times do |x|
				other[x, y] = yield(x, y)
			end
		end
		
		other
	end
	
	def [](x, y)
		x %= @width
		y %= @height
		@values[x + @width * y]
	end
	
	def []=(x, y, value)
		x %= @width
		y %= @height
		@values[x + @width * y] = value
	end
	
	def grad(x, y)
		# TODO
		# I don't foresee an application of this yet.
	end
	
	def laplacian(x, y)
		(self[x + 1, y] - 2 * self[x, y] + self[x - 1, y]) / DELTA_X**2 +
			(self[x, y + 1] - 2 * self[x, y] + self[x, y - 1]) / DELTA_Y**2
	end
	
	def print_grid
		@height.times do |y|
			@width.times do |x|
				print '%6.2f ' % self[x, y]
			end
			puts
		end
	end
	
	def print_dots
		@height.times do |y|
			@width.times do |x|
				print (yield(self[x, y]) ? '. ' : '  ')
			end
			puts
		end
	end
	
	def save(file)
		header = [@width, @height]
		file.write(header.pack('LL'))
		file.write(@values.pack('f*'))
		nil
	end
	
	def self.load(file)
		width, height = file.read(8).unpack('LL')
		field = Field.new(width, height)
		field.values = file.read(4 * width * height).unpack('f*')
		field
	end
end
