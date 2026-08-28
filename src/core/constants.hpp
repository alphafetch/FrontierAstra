#ifndef CORE_CONSTANTS_HPP
#define CORE_CONSTANTS_HPP

// QOL definitions
#define NEWLINE "\n"
#define ZERO_VEC3 glm::vec3(0, 0, 0)

// Player-related definitions
#define SPEED 1.5f
#define FOV 65.0f
#define NEAR_CLIP 0.1f
#define FAR_CLIP 100.0f
#define LOW_PITCH_CLAMP -85.0f
#define HIGH_PITCH_CLAMP 85.0f
#define SENSITIVITY 0.13f

// Screen definitions
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define WIN_TITLE "Frontier Astra"
#define SCR_CLEAR_COLOR 0.1f, 0.1f, 0.15f, 1.0f

// Game-related definitions
#define FIXED_TIME (1.0f / 60.0f)
#define MASTER_SEED 9801
// Noise
// - GLOBAL
#define GLOBAL_NOISE_TYPE FastNoiseLite::NoiseType_OpenSimplex2
#define GLOBAL_FRACTAL_TYPE FastNoiseLite::FractalType_FBm
// - PLANET
#define PLANET_FREQ 0.02f
#define PLANET_OCTAVES 2
#define PLANET_LACUNARITY 1.8f
#define PLANET_FRACTAL_GAIN 0.5f
#define PLANET_BASE_RADIUS 1.0f
#define PLANET_NOISE_SCALE_MIN 125
#define PLANET_NOISE_SCALE_MAX 250
#define PLANET_HEIGHT_SCALE_MIN 0.05f
#define PLANET_HEIGHT_SCALE_MAX 0.15f

// Debug definitions
#define LOG_LEN 512

// Filepath definitions
// Shaders
#define BASIC_VERT_SHADER "assets/shaders/basic.vert"
#define BASIC_FRAG_SHADER "assets/shaders/basic.frag"
// Textures
#define CONTAINER_JPG_TEX "assets/img/container.jpg"

#endif