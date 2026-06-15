#pragma once

#include <SDL3/SDL.h>
class Game;


class IWindowMode {
public:
	virtual ~IWindowMode() = default;

	[[nodiscard]] virtual SDL_AppResult CreateWindowAndRenderer(Game* game) = 0;

	virtual void GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) = 0;
};

class NativeWindowMode : public IWindowMode {
public:
	[[nodiscard]] SDL_AppResult CreateWindowAndRenderer(Game* game) override;

	void GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) override;
};

/// Transparent Fullscreen
class FakeWindowMode : public IWindowMode {
public:
	[[nodiscard]] SDL_AppResult CreateWindowAndRenderer(Game* game) override;

	void GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) override;
};
