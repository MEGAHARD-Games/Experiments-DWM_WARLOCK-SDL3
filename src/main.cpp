#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "Game.hpp"
#include "utils.hpp"


SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
	Game* game = new Game();
	*appstate = game;

	CHECK(game->InitWindow());

	CHECK(game->InitBackground());

	return SDL_APP_CONTINUE;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	Game* game = static_cast<Game*>(appstate);

	CHECK(game->HandleEvent(event));

	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void* appstate) {
	Game* game = static_cast<Game*>(appstate);

	CHECK(game->RenderFrame());

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, [[maybe_unused]] SDL_AppResult result) {
	const Game* state = static_cast<Game*>(appstate);
	state->Cleanup();
	delete state;
	SDL_Quit();
}
