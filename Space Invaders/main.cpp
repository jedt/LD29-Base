#include "GameEngine.hpp"
#include "PlayState.hpp"

int main ()
{
	// initialize the engine
	GameEngine game( "Engine Test v2.0" );
	game.run( game.build<PlayState>( true ) );
	
	// main loop
	while( game.running() )
	{
		game.nextState();
		game.update();
		game.draw();
	}
	
	// leaving the scope of 'game' will cleanup the engine
    return 0;
}