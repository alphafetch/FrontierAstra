#ifndef CORE_CONSTANTS_HPP
#define CORE_CONSTANTS_HPP

/*
Definitions for important game components.
(TD) - Dial that is likely to no longer be in the codebase at some point.
(D) - Tunable dial used for certain game logic or generation.
(F) - Set value, do not change.
(TF) - Temporary set value, still do not change.
(STCD) - In development and subject to change. Less likely to change than a dial.
*/

// QOL definitions
#define NEWLINE "\n" // Newline character
#define ZERO_VEC3 glm::vec3(0, 0, 0) // Zeroed Vector3
#define ZERO_DVEC3 glm::dvec3(0, 0, 0) // Zeroed Double Vector3

// Player-related definitions
#define SPEED 1.5f // Player speed (D)
#define FOV 65.0f // Player FOV (D)
#define NEAR_CLIP 0.1f // Near mesh clip (F)
#define FAR_CLIP 100.0f // Far mesh clip (F)
#define LOW_PITCH_CLAMP -85.0f // Camera pitch clamps (F)
#define HIGH_PITCH_CLAMP 85.0f
#define SENSITIVITY 0.13f // Mouse sens. (D)

// Screen definitions
#define SCR_WIDTH 800 // Screen size (STCD)
#define SCR_HEIGHT 600
#define SCR_TITLE "Frontier Astra" // Title (STCD)
#define SCR_CLEAR_COLOR 0.1f, 0.1f, 0.15f, 1.0f // BG color (TD)

// Game-related definitions
// - GLOBAL
#define FIXED_TIME (1.0f / 60.0f) // 60fps fixed update (F)
#define MASTER_SEED 9801 // Master seed (STCD)
#define GLOBAL_NOISE_TYPE FastNoiseLite::NoiseType_OpenSimplex2 // Noise types (F)
#define GLOBAL_FRACTAL_TYPE FastNoiseLite::FractalType_FBm // (F)
#define GLOBAL_LOD_REFRESH_DIST 10.0f // Distance to trigger an LOD refresh - higher = better performance (STCD)
// - PLANET
#define PLANET_COUNT 2 // Planet count (TD)
#define PLANET_FREQ 0.02f // Frequency for planet mesh generation (D)
#define PLANET_OCTAVES 2 // Octaves, lower, bigger details (D)
#define PLANET_LACUNARITY 1.8f // Noise - lacunarity (D)
#define PLANET_FRACTAL_GAIN 0.5f // Noise - fractal gain factor (D)
#define PLANET_BASE_RADIUS 1.0f // Radius for a planet (STCD)
#define PLANET_NOISE_SCALE_MIN 125 // Min for planet noise factor (D)
#define PLANET_NOISE_SCALE_MAX 250 // Max for planet noise factor (D)
#define PLANET_HEIGHT_SCALE_MIN 0.05f // Min for planet height factor (D)
#define PLANET_HEIGHT_SCALE_MAX 0.15f // Max for planet height factor (D)
#define PLANET_FACE_FULL_SIZE 2.0f // Do not change, base value for planet face (STCD)

// Optimization definitions
#define PLANET_QUADNODE_DISTANCE_FACTOR 1.75 // Tune for farther subdivision distance (STCD)
#define PLANET_QUADNODE_MAX_DEPTH 9 // 512 Resolution (D)
#define PLANET_QUADNODE_LEAF_RESOLUTION 64 // Individual resolution for leaves (STCD)
#define OCTREE_MAX_OBJECTS 50 // Maximum objects in an octree node (STCD)
#define OCTREE_MAX_DEPTH 6 // Maximum depth of one octree (STCD)
#define OCTREE_ROOT_SIZE 100000.0f // Unit size of a solar system (STCD)

// Debug definitions
#define LOG_LEN 512 // Length of logs for shader failures, etc. (F)

// Filepath definitions
// Shaders
#define BASIC_VERT_SHADER "assets/shaders/basic.vert" // BASIC shader (TF)
#define BASIC_FRAG_SHADER "assets/shaders/basic.frag"
// Textures
#define CONTAINER_JPG_TEX "assets/img/container.jpg" // Container (crate) (TF)

#endif