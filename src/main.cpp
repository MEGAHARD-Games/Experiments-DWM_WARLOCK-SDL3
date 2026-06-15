#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_common.hpp>

float map(const float value, const float inMin, const float inMax, const float outMin, const float outMax) {
	return outMin + (outMax - outMin) * ((value - inMin) / (inMax - inMin));
}

float random(const float min, const float max) {
	return map(SDL_randf(), 0.0f, 1.0f, min, max);
}

int random(const int min, const int max) {
	return SDL_rand(max - min + 1) + min;
}

glm::vec2 fromAngle(const float angle) {
	return {SDL_cos(angle), SDL_sin(angle)};
}

constexpr glm::ivec2 WINDOW_SIZE = {400, 400};

struct MyAppState {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* background = nullptr;
	glm::ivec2 displaySize = {};
};

SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
	MyAppState* state = new MyAppState();
	*appstate = state;

	if (!SDL_CreateWindowAndRenderer("DWM_WARLOCK (SDL Experiment)", 1, 1, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS, &state->window, &state->renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	const SDL_DisplayID gameDisplay = SDL_GetDisplayForWindow(state->window);
	SDL_Rect rect;
	SDL_GetDisplayBounds(gameDisplay, &rect);
	state->displaySize = {rect.w, rect.h};

	SDL_Renderer* renderer = state->renderer;
	state->background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, state->displaySize.x, state->displaySize.y);
	if (!state->background) {
		SDL_Log("Couldn't create render target: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetRenderTarget(renderer, state->background);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	for (int clumps = 0; clumps < 200; clumps++) {
		glm::vec2 clumpPos = {random(0.0f, static_cast<float>(state->displaySize.x)), random(0.0f, static_cast<float>(state->displaySize.y))};
		for (int blade = 0; blade < random(2, 3); blade++) {
			glm::vec2 bladeDir = fromAngle(glm::pi<float>() + glm::half_pi<float>() + random(-glm::quarter_pi<float>(), glm::quarter_pi<float>())); //random angle like:  \|/
			bladeDir *= random(5.0f, 10.0f); //length of the blade
			const glm::vec2 bladeEnd = clumpPos + bladeDir;
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderLine(renderer, clumpPos.x + random(-1.0f, 1.0f), clumpPos.y, bladeEnd.x, bladeEnd.y);
		}
	}

	SDL_SetRenderTarget(renderer, nullptr);

	return SDL_APP_CONTINUE;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	[[maybe_unused]] const MyAppState* state = static_cast<MyAppState*>(appstate);
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		case SDL_EVENT_KEY_DOWN:
			switch (event->key.key) {
				case SDLK_ESCAPE:
				case SDLK_Q:
					return SDL_APP_SUCCESS;
				default: break;
			}
		default: break;
	}
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void* appstate) {
	const MyAppState* state = static_cast<MyAppState*>(appstate);
	SDL_Renderer* renderer = state->renderer;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
	SDL_RenderClear(renderer);

	glm::vec2 mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	const glm::vec2 center = mouse - glm::vec2(WINDOW_SIZE / 2);
	const glm::vec2 maxClamp = state->displaySize - WINDOW_SIZE;
	const glm::vec2 clamped = glm::clamp(center, glm::vec2(0), maxClamp);

	const SDL_FRect rect = {clamped.x, clamped.y, WINDOW_SIZE.x, WINDOW_SIZE.y};
	SDL_RenderTexture(renderer, state->background, &rect, &rect);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, [[maybe_unused]] SDL_AppResult result) {
	const MyAppState* state = static_cast<MyAppState*>(appstate);
	SDL_DestroyRenderer(state->renderer);
	SDL_DestroyWindow(state->window);
	delete state;
	SDL_Quit();
}
