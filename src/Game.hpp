#pragma once

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
class IWindowMode;


class Game {
public:
	const glm::ivec2 WINDOW_SIZE = {400, 400};

	IWindowMode* windowMode = nullptr;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* background = nullptr;
	SDL_Rect displayBounds = {};

	[[nodiscard]] SDL_AppResult InitWindow();

	[[nodiscard]] SDL_AppResult InitBackground();

	[[nodiscard]] static SDL_AppResult HandleEvent(const SDL_Event* event);

	[[nodiscard]] SDL_AppResult RenderFrame();

	void Cleanup() const;
};
