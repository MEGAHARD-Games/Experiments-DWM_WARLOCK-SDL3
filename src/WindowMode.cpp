#include "WindowMode.hpp"

#include "Game.hpp"


SDL_AppResult NativeWindowMode::CreateWindowAndRenderer(Game* game) {
	if (!SDL_CreateWindowAndRenderer("DWM_WARLOCK (SDL Experiment)", game->WINDOW_SIZE.x, game->WINDOW_SIZE.y, SDL_WINDOW_ALWAYS_ON_TOP, &game->window, &game->renderer)) {
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
