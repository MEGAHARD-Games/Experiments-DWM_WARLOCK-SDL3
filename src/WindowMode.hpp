#pragma once

#include <SDL3/SDL.h>
class Game;


class IWindowMode {
public:
	virtual ~IWindowMode() = default;

	[[nodiscard]] virtual SDL_AppResult CreateWindowAndRenderer(Game* game) = 0;

	virtual void GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) = 0;

	virtual void SwitchTo(Game* game) = 0;
};

class NativeWindowMode : public IWindowMode {
	const char* const WINDOW_TITLE = "DWM_WARLOCK (Native)";

public:
	[[nodiscard]] SDL_AppResult CreateWindowAndRenderer(Game* game) override;

	void GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) override;

	void SwitchTo(Game* game) override;
};

/// Transparent Fullscreen
class FakeWindowMode : public IWindowMode {
	const char* const WINDOW_TITLE = "DWM_WARLOCK (Fake)";
public:
	[[nodiscard]] SDL_AppResult CreateWindowAndRenderer(Game* game) override;

	void GenerateBackgroundCopyRects(Game* game, SDL_FRect* srcRect, SDL_FRect* dstRect) override;

	void SwitchTo(Game* game) override;
};
