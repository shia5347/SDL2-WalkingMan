#include<math.h>
#include<stdio.h>
#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

bool Running = false;
SDL_Renderer* render;
SDL_Window* window;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Rect viewport;
SDL_Rect InfoBOX;

//Positions of sprites
int PlayerPOS [] = {0,0};
int BoxPOS [] = {40,54};

SDL_Event event;
TTF_Font* gFont;

SDL_Color textColour = {255,255,255};
SDL_Color menutextColour = {255,255,255};

int textWidth;
int textHeight;

//index 0 = Infobox
int FLAGS[] = {0,0,0};

class LSprite {	
	
	public:
	LSprite();		

	void Free();	

	bool Load(std::string fileName, SDL_Renderer* pRenderer);
	
	bool LoadText(std::string Type, SDL_Color textColor);	

	void Draw(int x, int y, int Width, int Height, SDL_Renderer* pRenderer);
	
	bool HandleCollisions();	
		
	private:	
	
	SDL_Texture* mTexture;
};



//The player sprite
LSprite Player;
//Box sprite
LSprite Box;
//Text Sprite
LSprite Text;

LSprite::LSprite() {

	mTexture = NULL;

}


void Init() {

if(gFont == NULL) {
	printf("Could not find font!");	
}

viewport.x = 0;
viewport.y = 0;

viewport.w = 640;
viewport.h = 480;

//Initialize info box
InfoBOX = {SCREEN_WIDTH/4,SCREEN_HEIGHT/4,SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
}

void handleEvents() {

	if(SDL_PollEvent(&event)) {
	
	switch(event.key.keysym.sym) {
	
		case SDLK_DOWN:
		//printf("Down key is pressed - move player down");
		PlayerPOS[1] += 4;
		//Player.Free();
		break;
		case SDLK_UP:
		PlayerPOS[1] -= 4;
		break;		
		case SDLK_RIGHT:
		PlayerPOS[0] += 4;
		break;
		case SDLK_LEFT:
		PlayerPOS[0] -= 4;
		break;
		case SDLK_ESCAPE:
		Running = false;
		break;
		case SDLK_F1:
		if(FLAGS[0] == 0){
		FLAGS[0] = 1;
		}
		break;
		case SDLK_BACKSPACE:
		FLAGS[0] = 0; 
		break;	
	}	
	
	}

}

void clean() {

	printf("Cleaning SDL systems");	
		
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);
	SDL_Quit();

}

bool LSprite::Load(std::string fileName, SDL_Renderer* pRenderer) {

	SDL_Surface* tempSurface = IMG_Load(fileName.c_str());

	if(tempSurface == NULL) {
	
	return false;	
	
	}

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer,tempSurface);
	SDL_FreeSurface(tempSurface);

	if(pTexture != NULL) {
	
	mTexture = pTexture;
	return true;	

	} else {
	
	return false;

	}	

}

bool LSprite::LoadText(std::string Type, SDL_Color textColour) {

	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont,Type.c_str(),textColour,350);

	if(textSurface == NULL) {
	
	return false;	
	
	}

	mTexture = SDL_CreateTextureFromSurface(render,textSurface);

	if(mTexture == NULL) {
	
	return false;	
	
	}

	textWidth = textSurface->w;
	textHeight = textSurface->h;


	SDL_FreeSurface(textSurface);

	return mTexture != NULL;

}

void LSprite::Draw(int x, int y, int Width, int Height, SDL_Renderer* pRenderer, SDL_Rect Collisionbox) {
	
	SDL_Rect desRect;

	desRect.w = Width;
	
	desRect.h = Height;

	desRect.x = x;
	desRect.y = y;

	SDL_RenderCopy(pRenderer,mTexture,NULL,&desRect);


}

void LSprite::Free() {

	if(mTexture != NULL) {
	
	SDL_DestroyTexture(mTexture);
	mTexture = NULL;	
	
	}
}

bool LSprite::HandleCollisions(){

		

}

void Render() {

	SDL_RenderClear(render);
	SDL_RenderSetViewport(render,&viewport);
	//Player sprite loop
	Player.Load("sprites/Player.png",render);
	Player.Draw(PlayerPOS[0],PlayerPOS[1],20,20,render);
	Player.Free();

	//Box sprite loop
	Box.Load("sprites/Box.png",render);
	Box.Draw(BoxPOS[0],BoxPOS[1],20,20,render);
	Box.Free();
	
	//Text sprite loop
	Text.LoadText("ESC to quit",textColour);
	Text.Draw(0,480-textHeight,textWidth,textHeight,render);
	Text.Free();	

	//Box-info geometry loop	
	if(FLAGS[0] > 0) {
	SDL_SetRenderDrawColor(render,49,0,121,NULL);
	SDL_RenderFillRect(render,&InfoBOX);
	
	//The text in the box
	Text.LoadText("Welcome to this random game please enjoy",menutextColour);
	Text.Draw(SCREEN_WIDTH/4,SCREEN_HEIGHT/4,textWidth,textHeight,render);
	Text.Free();	
	}

	
	SDL_RenderPresent(render);

}

void Update() {
	
	


}

int main() {
		

	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
	
	window = SDL_CreateWindow("TestGame",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);	
	
	}

	if(window != 0 ) {
	
	render = SDL_CreateRenderer(window,-1,0);
	SDL_SetRenderDrawColor(render,0,0,0,255);

	TTF_Init();
	gFont = TTF_OpenFont("Fonts/carbon bl.ttf",20);
	
		

	} else {
	return -1;	
	}
	
	Running = true;	
	while(Running) {
	
		
	handleEvents();
	Init();	
	Render();	
	
		
	}			
	clean();
	return 0;
	}
