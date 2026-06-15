#include "WindowMode.hpp"

#include <glm/ext/scalar_common.hpp>

#include "Game.hpp"


// --- Native Window Mode

SDL_AppResult NativeWindowMode::CreateWindowAndRenderer(Game* game) {
	if (!SDL_CreateWindowAndRenderer("DWM_WARLOCK/SDL3 (Native)", game->WINDOW_SIZE.x, game->WINDOW_SIZE.y, SDL_WINDOW_ALWAYS_ON_TOP, &game->window, &game->renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

void NativeWindowMode::GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) {
	int x, y;
	SDL_GetWindowPosition(game->window, &x, &y);
	x -= game->displayBounds.x;
	y -= game->displayBounds.y;
	const float fx = static_cast<float>(x);
	const float fy = static_cast<float>(y);

	const float windowWidth = static_cast<float>(game->WINDOW_SIZE.x);
	const float windowHeight = static_cast<float>(game->WINDOW_SIZE.y);
	*srcRect = {fx, fy, windowWidth, windowHeight};

	const float insetX = SDL_max(0.0f, 0.0f - fx);
	const float insetY = SDL_max(0.0f, 0.0f - fy);

	const float outsetX = SDL_min(0.0f, static_cast<float>(game->displayBounds.w) - fx - windowWidth);
	const float outsetY = SDL_min(0.0f, static_cast<float>(game->displayBounds.h) - fy - windowHeight);

	*dstRect = {
		insetX,
		insetY,
		windowWidth - insetX + outsetX,
		windowHeight - insetY + outsetY,
	};
}


// --- Fake Window Mode

SDL_AppResult FakeWindowMode::CreateWindowAndRenderer(Game* game) {
	if (!SDL_CreateWindowAndRenderer("DWM_WARLOCK/SDL (Fake)", 1, 1, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS, &game->window, &game->renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

void FakeWindowMode::GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
	SDL_RenderClear(game->renderer);

	glm::vec2 mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	const glm::vec2 windowSize = game->WINDOW_SIZE;
	const glm::vec2 center = mouse - windowSize / 2.0f;
	const glm::vec2 maxClamp = glm::vec2{game->displayBounds.w, game->displayBounds.h} - windowSize;
	const glm::vec2 clamped = glm::clamp(center, glm::vec2(0), maxClamp);

	const SDL_FRect rect = {clamped.x, clamped.y, windowSize.x, windowSize.y};
	*srcRect = rect;
	*dstRect = rect;
}
