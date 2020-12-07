#include "Stdafx.h"
/*This source code copyrighted by Lazy Foo' Productions (2004-2009) and may not
be redestributed without written permission.*/

// ���带 �̿��ϱ� ���� ����ü
Mix_Music *Music = NULL;

// ȿ������ �̿��ϱ� ���� ����ü
Mix_Chunk *Effect = NULL;
Mix_Chunk *Effect_Create = NULL;

SDL_Surface* bmp;
SDL_Surface* player;

Uint32 colorkey;

SDL_Rect dstrect;
SDL_Surface* screen;

TTF_Font* font;
SDL_Surface* msg;

void PlaySoundEffect(/*char* filename*/){
	//Load the Sound Effect
	
	Mix_PlayChannel(-1, Effect, 0);
}

void PlaySoundEffectCreate(/*char* filename*/){
	//Load the Sound Effect
	
	Mix_PlayChannel(-1, Effect_Create, 0);
}

bool SDL_Init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

//	screen= SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

	TTF_Init();

	dstrect.x = SCR_WIDTH / 2;
	dstrect.y = SCR_HEIGHT / 2;

    //Initialize SDL_mixer
	//SDL_mixer�ʱ�ȭ ������ ���� �ʱ�ȭ �Լ����� Mix_OpenAudio()��� �Լ��� ȣ���� �ְ� �ִµ� �Լ� ������ ������ �����ϴ�.
	//int Mix_OpenAudio(int frequency(���ļ�), Uint16 format(����), int channels, int chunksize);

    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

//	colorkey = SDL_MapRGB(msg->format, 0, 0, 0);
//	SDL_SetColorKey(player, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);

	// ȿ���� ���� �̸� �ε�
	Effect = Mix_LoadWAV("./Sound/Effect.wav");
	Effect_Create = Mix_LoadWAV("./Sound/Effect_Create.wav");

    //If everything initialized fine
    return true;
}

void SDL_Show(){
//	apply_surface( 0, 200, msg, screen ); 
	SDL_BlitSurface(msg, 0, screen, &dstrect);
	SDL_Flip(screen);
}

bool SDL_Load_Text(char* str)
{
	font = TTF_OpenFont("HARNGTON.ttf", 30);

	SDL_Color FontColor = {255, 255, 255};

	msg = TTF_RenderText_Blended(font, str, FontColor);

	SDL_Show();

	TTF_CloseFont(font);
    //If everything loaded fine
    return true;
}

void SDL_Release()
{
	SDL_FreeSurface(bmp);
	SDL_FreeSurface(player);
    //Free the sound effects
	Mix_FreeChunk(Effect);
    //Free the sound effects
    Mix_FreeMusic(Music);

    //Quit SDL_mixer
    Mix_CloseAudio();

	TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

//
//int main( int argc, char* args[] )
//{
//    //Quit flag
//    bool quit = false;
//
//    //Initialize
//    if( init() == false )
//    {
//        return 1;
//    }
//
//    //Load the files
//    if( load_files() == false )
//    {
//        return 1;
//    }
//
//    //While the user hasn't quit
//    while( quit == false )
//    {
//        //While there's events to handle
//        while( SDL_PollEvent( &event ) )
//        {
//            //If a key was pressed
//            if( event.type == SDL_KEYDOWN )
//            {
//                //If 1 was pressed
//                if( event.key.keysym.sym == SDLK_1 )
//                {
//                    //Play the scratch effect
//					// ������ ���ڴ� �ݺ�Ƚ��, 0�̸� �� ��, -1 �̸� ���� �ݺ�
//                    if( Mix_PlayChannel( -1, scratch, 0 ) == -1 )
//                    {
//                        return 1;
//                    }
//                }
//                //If 2 was pressed
//                else if( event.key.keysym.sym == SDLK_2 )
//                {
//                    //Play the high hit effect
//                    if( Mix_PlayChannel( -1, high, 0 ) == -1 )
//                    {
//                        return 1;
//                    }
//                }
//                //If 3 was pressed
//                else if( event.key.keysym.sym == SDLK_3 )
//                {
//                    //Play the medium hit effect
//                    if( Mix_PlayChannel( -1, med, 0 ) == -1 )
//                    {
//                        return 1;
//                    }
//                }
//                //If 4 was pressed
//                else if( event.key.keysym.sym == SDLK_4 )
//                {
//                    //Play the low hit effect
//                    if( Mix_PlayChannel( -1, low, 0 ) == -1 )
//                    {
//                        return 1;
//                    }
//                }
//                //If 9 was pressed
//                else if( event.key.keysym.sym == SDLK_9 )
//                {
//                    //If there is no music playing
//                    if( Mix_PlayingMusic() == 0 )
//                    {
//                        //Play the music
//                        if( Mix_PlayMusic( music, -1 ) == -1 )
//                        {
//                            return 1;
//                        }
//                    }
//                    //If music is being played
//                    else
//                    {
//                        //If the music is paused
//                        if( Mix_PausedMusic() == 1 )
//                        {
//                            //Resume the music
//                            Mix_ResumeMusic();
//                        }
//                        //If the music is playing
//                        else
//                        {
//                            //Pause the music
//                            Mix_PauseMusic();
//                        }
//                    }
//                }
//                //If 0 was pressed
//                else if( event.key.keysym.sym == SDLK_0 )
//                {
//                    //Stop the music
//                    Mix_HaltMusic();
//                }
//            }
//            //If the user has Xed out the window
//            if( event.type == SDL_QUIT )
//            {
//                //Quit the program
//                quit = true;
//            }
//        }
//    }
//
//    //Free surfaces, fonts and sounds
//    //then quit SDL_mixer, SDL_ttf and SDL
//    clean_up();
//
//    return 0;
//}
