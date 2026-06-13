#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>

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

constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 400;

struct MyAppState {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* background = nullptr;
	glm::ivec2 displaySize = {};
};

SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
	MyAppState* state = new MyAppState();
	*appstate = state;

	if (!SDL_CreateWindowAndRenderer("DWM_WARLOCK (SDL Experiment)", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP, &state->window, &state->renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	SDL_Rect rect;
	SDL_GetDisplayBounds(primaryDisplay, &rect);
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
	SDL_Window* window = state->window;
	SDL_Renderer* renderer = state->renderer;

	int x, y;
	SDL_GetWindowPosition(window, &x, &y);

	const SDL_FRect srcRect = {static_cast<float>(x), static_cast<float>(y), WINDOW_WIDTH, WINDOW_HEIGHT};

	const float insetX = SDL_max(0, 0 - x);
	const float insetY = SDL_max(0, 0 - y);

	const float outsetX = SDL_min(0, static_cast<float>(state->displaySize.x) - static_cast<float>(x) - WINDOW_WIDTH);
	const float outsetY = SDL_min(0, static_cast<float>(state->displaySize.y) - static_cast<float>(y) - WINDOW_HEIGHT);

	const SDL_FRect dstRect = {insetX, insetY, WINDOW_WIDTH - insetX + outsetX, WINDOW_HEIGHT - insetY + outsetY};

	SDL_RenderTexture(renderer, state->background, &srcRect, &dstRect);

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
