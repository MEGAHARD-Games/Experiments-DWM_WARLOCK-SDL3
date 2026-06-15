#pragma once

#include <SDL3/SDL.h>

constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 400;

class Game {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* background = nullptr;
	SDL_Rect displayBounds = {};

public:
	[[nodiscard]] SDL_AppResult InitWindow();

	[[nodiscard]] SDL_AppResult InitBackground();

	[[nodiscard]] static SDL_AppResult HandleEvent(const SDL_Event* event);

	[[nodiscard]] SDL_AppResult RenderFrame() const;

	void Cleanup() const;
};
