#ifndef CONFIG_H
#define CONFIG_H


// The world grid dimensions
// Dimensions must be a power of two for now.
#define WORLD_W 1024
#define WORLD_H 512

// The spacing between successive timesteps or cells
#define DELTA_TIME 0.1f
#define DELTA_X 1.0f
#define DELTA_Y 1.0f

// The field layer definitions
enum field_type_e
{
	ELEVATION,
	PREY_POPULATION,
	PREDATOR_POPULATION,
	
	// Keep this at the end
	FIELD_TYPE_LENGTH
};

#endif