#ifndef CORE_CONSTANTS_HPP
#define CORE_CONSTANTS_HPP

// QOL definitions
#define NEWLINE "\n" // Newline character
#define ZERO_VEC3 glm::vec3(0, 0, 0) // Zeroed Vector3

// Player-related definitions
#define SPEED 1.5f // Player speed
#define FOV 65.0f // Player FOV
#define NEAR_CLIP 0.1f // Near mesh clip
#define FAR_CLIP 100.0f // Far mesh clip
#define LOW_PITCH_CLAMP -85.0f // Camera pitch clamps
#define HIGH_PITCH_CLAMP 85.0f
#define SENSITIVITY 0.13f // Mouse sens.

// Screen definitions
#define SCR_WIDTH 800 // Screen size
#define SCR_HEIGHT 600
#define WIN_TITLE "Frontier Astra" // Title
#define SCR_CLEAR_COLOR 0.1f, 0.1f, 0.15f, 1.0f // BG color

// Game-related definitions
// - GLOBAL
#define FIXED_TIME (1.0f / 60.0f) // 60fps fixed update
#define MASTER_SEED 9801 // Master seed
#define GLOBAL_NOISE_TYPE FastNoiseLite::NoiseType_OpenSimplex2 // Noise types
#define GLOBAL_FRACTAL_TYPE FastNoiseLite::FractalType_FBm
#define GLOBAL_LOD_REFRESH_DIST 5.0f
// - PLANET
#define PLANET_COUNT 2 // Planet count (temporary dial)
#define PLANET_FREQ 0.02f // Frequency for planet generation
#define PLANET_OCTAVES 2 // Octaves, lower, bigger details
#define PLANET_LACUNARITY 1.8f // Noise - lacunarity
#define PLANET_FRACTAL_GAIN 0.5f // Noise - fractal gain factor
#define PLANET_BASE_RADIUS 1.0f // Radius for a planet
#define PLANET_NOISE_SCALE_MIN 125 // Min for planet noise factor
#define PLANET_NOISE_SCALE_MAX 250 // Max for planet noise factor
#define PLANET_HEIGHT_SCALE_MIN 0.05f // Min for planet height factor
#define PLANET_HEIGHT_SCALE_MAX 0.15f // Max for planet height factor
#define PLANET_FACE_FULL_SIZE 2.0f // Do not change, base value for planet face
#define PLANET_SUBDIVIDE_DIST_FACTOR 1.75 // Tune for farther subdivision distance
#define PLANET_MAX_SUB_DEPTH 9 // 512 Resolution
#define PLANET_LEAF_RES 64 // Individual resolution for leaves

// Debug definitions
#define LOG_LEN 512 // Length of logs for shader failures, etc.

// Filepath definitions
// Shaders
#define BASIC_VERT_SHADER "assets/shaders/basic.vert" // BASIC shader
#define BASIC_FRAG_SHADER "assets/shaders/basic.frag"
// Textures
#define CONTAINER_JPG_TEX "assets/img/container.jpg" // Container (crate)

#endif