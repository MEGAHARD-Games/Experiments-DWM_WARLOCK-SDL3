#pragma once

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>

#define CHECK(expr)\
	do { \
		const SDL_AppResult result = (expr); \
		if (result != SDL_APP_CONTINUE) \
			return result; \
	} while (false)

inline float map(const float value, const float inMin, const float inMax, const float outMin, const float outMax) {
	return outMin + (outMax - outMin) * ((value - inMin) / (inMax - inMin));
}

inline float random(const float min, const float max) {
	return map(SDL_randf(), 0.0f, 1.0f, min, max);
}

inline int random(const int min, const int max) {
	return SDL_rand(max - min + 1) + min;
}

inline glm::vec2 fromAngle(const float angle) {
	return {SDL_cos(angle), SDL_sin(angle)};
}
