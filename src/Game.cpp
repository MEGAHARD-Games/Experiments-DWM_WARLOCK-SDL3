#include "Game.hpp"

#include <glm/gtc/constants.hpp>

#include "utils.hpp"


SDL_AppResult Game::InitWindow() {
	if (!SDL_CreateWindowAndRenderer("DWM_WARLOCK (SDL Experiment)", WINDOW_SIZE.x, WINDOW_SIZE.y, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	const SDL_DisplayID gameDisplay = SDL_GetDisplayForWindow(window);
	SDL_GetDisplayBounds(gameDisplay, &displayBounds);

	return SDL_APP_CONTINUE;
}

SDL_AppResult Game::InitBackground() {
	background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, displayBounds.w, displayBounds.h);
	if (!background) {
		SDL_Log("Couldn't create render target: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetRenderTarget(renderer, background);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	for (int clumps = 0; clumps < 200; clumps++) {
		glm::vec2 clumpPos = {random(0.0f, static_cast<float>(displayBounds.w)), random(0.0f, static_cast<float>(displayBounds.h))};
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

SDL_AppResult Game::HandleEvent(const SDL_Event* event) {
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

SDL_AppResult Game::RenderFrame() const {
	int x, y;
	SDL_GetWindowPosition(window, &x, &y);
	x -= displayBounds.x;
	y -= displayBounds.y;
	const float fx = static_cast<float>(x);
	const float fy = static_cast<float>(y);

	const float windowWidth = static_cast<float>(WINDOW_SIZE.x);
	const float windowHeight = static_cast<float>(WINDOW_SIZE.y);
	const SDL_FRect srcRect = {fx, fy, windowWidth, windowHeight};

	const float insetX = SDL_max(0.0f, 0.0f - fx);
	const float insetY = SDL_max(0.0f, 0.0f - fy);

	const float outsetX = SDL_min(0.0f, static_cast<float>(displayBounds.w) - fx - windowWidth);
	const float outsetY = SDL_min(0.0f, static_cast<float>(displayBounds.h) - fy - windowHeight);

	const SDL_FRect dstRect = {
		insetX,
		insetY,
		windowWidth - insetX + outsetX,
		windowHeight - insetY + outsetY,
	};

	SDL_RenderTexture(renderer, background, &srcRect, &dstRect);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void Game::Cleanup() const {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
