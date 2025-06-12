//Alexander Young
//Lab 11

#include "SpriteSheet.h"

//This is a constructor for the Sprite class
//Takes no parameters
//No return
Sprite::Sprite()
{
	image=NULL;
}

//This is a deconstructor for the sprite class
//Takes no parameters
//No return
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}

//This function spawns the sprite in the display
//Takes two integers representing the display width and height
//No return
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = 10;


	maxFrame = 8;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("guy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

//This function updates a sprite's location and animation sequence on a screen
//Takes two integers representing the display width and height and an integer representing the direction
//No return
void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if(dir == 1){ //right key
		animationDirection = 1; 
		x+=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > maxFrame)
				curFrame=1;
		}
	} else if (dir == 0){ //left key
		animationDirection = 0; 
		x-=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > maxFrame)
				curFrame=1;
		}
	}else if (dir == 3) { //space bar
		animationDirection = 3;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > 12)
				curFrame = 9;
		}
	}
	else //represent that they hit the space bar and that mean direction = 0
		animationDirection = dir;

	//check for collided with foreground tiles
	if (animationDirection==0)
	{ 
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = oldx; 
			y= oldy;
		}

	}
	else if (animationDirection ==1)
	{ 
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = oldx; 
			y= oldy;
		}
	}
}

//This function detencts if a sprite collides with an endblock
//Takes no parameters
//Returns a boolean
bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth/2, y + frameHeight + 5))
		return true;
	else
		return false;
}

//This function draws a sprite at the x and y position on the display at the current animaton sequence
//Takes two integers representing the x and y offset
//No return
void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (animationDirection==1){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, 0);
	}else if (animationDirection == 0 ){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}else if (animationDirection == 2 ){
		al_draw_bitmap_region(image,0,0,frameWidth,frameHeight,  x-xoffset, y-yoffset, 0);
	}
	//change the player animation for jumping input
	else if (animationDirection == 3) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
}


//This function handles the jumping animation and limitations
//Takes two integers representing the jump/falling values
//returns an integer
int Sprite::jumping(int jump, const int JUMPIT)
{
	//handle jumping
	if (jump==JUMPIT) { 
		if (!collided(x + frameWidth/2, y + frameHeight + 5))
			jump = 0; 
	}
	else
	{
		//check to see if the player is within the boundaries of the screen height, do not let them go above the top of the display
		if (jump > 0) {
			if (y > 0) {
				y -= jump / 3;
			}
			else {
				jump = 0;
			}
		}
		else {
			y -= jump / 3;
		}
		jump--;
		curFrame=8;
	}

	if (jump<0) 
	{ 
		if (collided(x + frameWidth/2,  y + frameHeight))
		{ 
			jump = JUMPIT; 
			while (collided(x + frameWidth/2,y + frameHeight))
			{
				y -= 3;
			}
		} 
	}
	return jump;
}