require_relative 'field'


if ARGV.length < 4
	puts "usage: <terrain_dir> <field_filename> <width> <height>"
	exit 1
end


TERRAIN_DIR = ARGV[0]
TERRAIN_FILENAMES = %w{
	a11g b10g c10g d10g
	e10g f10g g10g h10g
	i10g j10g k10g l10g
	m10g n10g o10g p10g
}
WORLD_WIDTH = 4 * 10800
WORLD_HEIGHT = 4800 + 2 * 6000 + 4800
BLOCK_WIDTH = 10800

TERRAIN_FILES = TERRAIN_FILENAMES.collect do |filename|
	File.open(File.realpath(filename, TERRAIN_DIR))
end


def get_row(row)
	data = []
	
	block_y = case row
	when 0...4800
		0
	when 4800...10800
		row -= 4800
		1
	when 10800...16800
		row -= 10800
		2
	when 16800...21600
		row -= 16800
		3
	end
	
	for block_x in 0...4
		terrain_file = TERRAIN_FILES[block_x + 4 * block_y]
		terrain_file.seek(2 * row * BLOCK_WIDTH, IO::SEEK_SET)
		data += terrain_file.read(2 * BLOCK_WIDTH).unpack('s<*')
	end
	
	data
end


field_width = ARGV[2].to_i
field_height = ARGV[3].to_i
field = Field.new(field_width, field_height)

field_height.times do |y|
	row = y * WORLD_HEIGHT / field_height
	row_data = get_row(row)
	
	field_width.times do |x|
		col = x * WORLD_WIDTH / field_width
		field[x, y] = row_data[col]
	end
end

field_file = File.open(ARGV[1], 'w')
# field.print_dots {|v| v > 0}
field.save(field_file)