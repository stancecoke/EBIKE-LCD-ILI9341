//	MIT License
//
//	Copyright (c) 2017 Matej Artnak
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
//
//
//
//-----------------------------------
//	ILI9341 GFX library for STM32
//-----------------------------------
//
//	Very simple GFX library built upon ILI9342_STM32_Driver library.
//	Adds basic shapes, image and font drawing capabilities to ILI9341
//
//	Library is written for STM32 HAL library and supports STM32CUBEMX. To use the library with Cube software
//	you need to tick the box that generates peripheral initialization code in their own respective .c and .h file
//
//
//-----------------------------------
//	How to use this library
//-----------------------------------
//
//	-If using MCUs other than STM32F7 you will have to change the #include "stm32f7xx_hal.h" in the ILI9341_GFX.h to your respective .h file
//
//	If using "ILI9341_STM32_Driver" then all other prequisites to use the library have allready been met
//	Simply include the library and it is ready to be used
//
//-----------------------------------


#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "5x5_font.h"
#include "spi.h"

/*Draw hollow circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ILI9341_Draw_Hollow_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{
	int x = Radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (Radius << 1);

    while (x >= y)
    {
        ILI9341_Draw_Pixel(X + x, Y + y, Colour);
        ILI9341_Draw_Pixel(X + y, Y + x, Colour);
        ILI9341_Draw_Pixel(X - y, Y + x, Colour);
        ILI9341_Draw_Pixel(X - x, Y + y, Colour);
        ILI9341_Draw_Pixel(X - x, Y - y, Colour);
        ILI9341_Draw_Pixel(X - y, Y - x, Colour);
        ILI9341_Draw_Pixel(X + y, Y - x, Colour);
        ILI9341_Draw_Pixel(X + x, Y - y, Colour);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += (-Radius << 1) + dx;
        }
    }
}

/*Draw filled circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ILI9341_Draw_Filled_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{
	
		int x = Radius;
    int y = 0;
    int xChange = 1 - (Radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = X - x; i <= X + x; i++)
        {
            ILI9341_Draw_Pixel(i, Y + y,Colour);
            ILI9341_Draw_Pixel(i, Y - y,Colour);
        }
        for (int i = X - y; i <= X + y; i++)
        {
            ILI9341_Draw_Pixel(i, Y + x,Colour);
            ILI9341_Draw_Pixel(i, Y - x,Colour);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
		//Really slow implementation, will require future overhaul
		//TODO:	https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles	
}

/*Draw a hollow rectangle between positions X0,Y0 and X1,Y1 with specified colour*/
void ILI9341_Draw_Hollow_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t 	X_length = 0;
	uint16_t 	Y_length = 0;
	uint8_t		Negative_X = 0;
	uint8_t 	Negative_Y = 0;
	float 		Calc_Negative = 0;
	
	Calc_Negative = X1 - X0;
	if(Calc_Negative < 0) Negative_X = 1;
	Calc_Negative = 0;
	
	Calc_Negative = Y1 - Y0;
	if(Calc_Negative < 0) Negative_Y = 1;
	
	
	//DRAW HORIZONTAL!
	if(!Negative_X)
	{
		X_length = X1 - X0;		
	}
	else
	{
		X_length = X0 - X1;		
	}
	ILI9341_Draw_Horizontal_Line(X0, Y0, X_length, Colour);
	ILI9341_Draw_Horizontal_Line(X0, Y1, X_length, Colour);
	
	
	
	//DRAW VERTICAL!
	if(!Negative_Y)
	{
		Y_length = Y1 - Y0;		
	}
	else
	{
		Y_length = Y0 - Y1;		
	}
	ILI9341_Draw_Vertical_Line(X0, Y0, Y_length, Colour);
	ILI9341_Draw_Vertical_Line(X1, Y0, Y_length, Colour);
	
	if((X_length > 0)||(Y_length > 0)) 
	{
		ILI9341_Draw_Pixel(X1, Y1, Colour);
	}
	
}
/*Draw a filled rectangle between positions X0,Y0 and X1,Y1 with specified colour*/
void ILI9341_Draw_Filled_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t 	X_length = 0;
	uint16_t 	Y_length = 0;
	uint8_t		Negative_X = 0;
	uint8_t 	Negative_Y = 0;
	int32_t 	Calc_Negative = 0;
	
	uint16_t X0_true = 0;
	uint16_t Y0_true = 0;
	
	Calc_Negative = X1 - X0;
	if(Calc_Negative < 0) Negative_X = 1;
	Calc_Negative = 0;
	
	Calc_Negative = Y1 - Y0;
	if(Calc_Negative < 0) Negative_Y = 1;
	
	
	//DRAW HORIZONTAL!
	if(!Negative_X)
	{
		X_length = X1 - X0;
		X0_true = X0;
	}
	else
	{
		X_length = X0 - X1;
		X0_true = X1;
	}
	
	//DRAW VERTICAL!
	if(!Negative_Y)
	{
		Y_length = Y1 - Y0;
		Y0_true = Y0;		
	}
	else
	{
		Y_length = Y0 - Y1;
		Y0_true = Y1;	
	}
	
	ILI9341_Draw_Rectangle(X0_true, Y0_true, X_length, Y_length, Colour);	
}

/*Draws a character (fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/


void ILI9341_DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Background_Colour)
{
	uint8_t charWidth;			/* Width of character */
	uint16_t fOffset = font[0];	/* Offset of character */
	uint8_t fWidth = font[1];	/* Width of font */
	//fOffset=337;
	while (*str)
	{
		ILI9341_DrawChar(*str, font, X, Y, Colour, Background_Colour);

		/* Check character width and calculate proper position */
		uint16_t *tempChar = (uint16_t*)&font[((*str - 0x20) * fOffset) + 4];
		charWidth = tempChar[0];

		if(charWidth + 2 < fWidth)
		{
			/* If character width is smaller than font width */
			X += (charWidth + 2);
		}
		else
		{
			X += fWidth;
		}

		str++;
	}
}
void ILI9341_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Background_Colour)
{
	if ((ch < 31) || (ch > 127)) return;

	uint16_t fOffset, fWidth, fHeight, fBPL;
	uint8_t *tempChar;

	fOffset = font[0];
	//fOffset=337;
	fWidth = font[1];
	fHeight = font[2];
	fBPL = font[3];
	uint16_t temp;

	tempChar = (uint8_t*)&font[((ch - 0x20) * fOffset) + 4]; /* Current Character = Meta + (Character Index * Offset) */

	/* Clear background first */
	ILI9341_Draw_Rectangle(X, Y, fWidth+4, fHeight, Background_Colour);

	for (int j=0; j < fHeight; j++)
	{
		for (int i=0; i < fWidth; i++)
		{temp=fBPL * i + ((j & 0xF8) >> 3);
			uint8_t z =  tempChar[ temp  + 1]; /* (j & 0xF8) >> 3, increase one by 8-bits */
			uint8_t b = 1 << (j & 0x07);
			if (( z & b ) != 0x00)
			{
				ILI9341_Draw_Pixel(X+i, Y+j, Colour);
			}
		}
	}
}

void ILI9341_DrawBigNumber(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Background_Colour)
{
	uint8_t charWidth;			/* Width of character */
	uint16_t fOffset = (font[0]<<8)+font[1];	/* Offset of character */
	uint8_t fWidth = font[2];	/* Width of font */

	while (*str)
	{
		ILI9341_DrawBigNumericalCharakter(*str, font, X, Y, Colour, Background_Colour);

		/* Check character width and calculate proper position */
		uint8_t *tempChar = (uint8_t*)&font[((*str - 0x30) * fOffset) + 5];
		charWidth = tempChar[0];

		if(charWidth + 2 < fWidth)
		{
			/* If character width is smaller than font width */
			X += (charWidth + 2);
		}
		else
		{
			X += fWidth;
		}

		str++;
	}
}

void ILI9341_DrawBigNumericalCharakter(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Background_Colour)
{
	if ((ch < 31) || (ch > 127)) return;

	uint16_t fOffset, fWidth, fHeight, fBPL;
	uint8_t *tempChar;

	fOffset = (font[0]<<8)+font[1];

	fWidth = font[2];
	fHeight = font[3];
	fBPL = font[4];


	tempChar = (uint8_t*)&font[((ch - 0x30) * fOffset) + 6]; // Current Character = Meta + (Character Index * Offset) Zeiger auf das gewünschte Zeichen

	/* Clear background first */
	ILI9341_Draw_Rectangle(X, Y, fWidth+4, fHeight, Background_Colour);


	//Idee: Spaltenweise abarbeiten
	for (int i=0; i < fWidth; i++){
		for(int j=0; j < fBPL; j++){
			uint8_t z =  tempChar[j];
			for(int k=0; k < 8; k++){
				 if ((z>>k)&1)
				{
					ILI9341_Draw_Pixel(X+i, Y+8*j+k, Colour);
				}
			}
		}
		tempChar += fBPL;
	}
}

void ILI9341_DrawICON(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Background_Colour)
{
	if ((ch < 31) || (ch > 127)) return;

	uint16_t fOffset, fWidth, fHeight, fBPL;
	uint8_t *tempChar;

	fOffset = (font[0]<<8)+font[1];

	fWidth = font[2];
	fHeight = font[3];
	fBPL = font[4];


	tempChar = (uint8_t*)&font[((ch - 0x30) * fOffset) + 5]; // Current Character = Meta + (Character Index * Offset) Zeiger auf das gewünschte Zeichen

	/* Clear background first */
	ILI9341_Draw_Rectangle(X, Y, fWidth+4, fHeight, Background_Colour);


	//Format aus Vertikaler Einstellung siehe https://javl.github.io/image2cpp/
	for (int j=0; j < fBPL; j++){
	for (int i=0; i < fWidth; i++){

			uint8_t z =  tempChar[i];
			for(int k=0; k < 8; k++){
				 if ((z>>k)&1)
				{
					ILI9341_Draw_Pixel(X+i, Y+8*j+k, Colour);
				}

		}
	}
	tempChar += fWidth;

	}
}



void ILI9341_Draw_Char(char Character, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour) 
{
		uint8_t 	function_char;
    uint8_t 	i,j;
		
		function_char = Character;
		
    if (function_char < ' ') {
        Character = 0;
    } else {
        function_char -= 32;
		}
   	
		char temp[CHAR_WIDTH];
		for(uint8_t k = 0; k<CHAR_WIDTH; k++)
		{
		temp[k] = font[function_char][k];
		}
		
    // Draw pixels
		ILI9341_Draw_Rectangle(X, Y, CHAR_WIDTH*Size, CHAR_HEIGHT*Size, Background_Colour);
    for (j=0; j<CHAR_WIDTH; j++) {
        for (i=0; i<CHAR_HEIGHT; i++) {
            if (temp[j] & (1<<i)) {			
							if(Size == 1)
							{
              ILI9341_Draw_Pixel(X+j, Y+i, Colour);
							}
							else
							{
							ILI9341_Draw_Rectangle(X+(j*Size), Y+(i*Size), Size, Size, Colour);
							}
            }						
        }
    }
}

/*Draws an array of characters (fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/
void ILI9341_Draw_Text(const char* Text, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour)
{
    while (*Text) {
        ILI9341_Draw_Char(*Text++, X, Y, Colour, Size, Background_Colour);
        X += CHAR_WIDTH*Size;
    }
}

/*Draws a full screen picture from flash. Image converted from RGB .jpeg/other to C array using online converter*/
//USING CONVERTER: http://www.digole.com/tools/PicturetoC_Hex_converter.php
//65K colour (2Bytes / Pixel)
void ILI9341_Draw_Image(const char* Image_Array, uint8_t Orientation)
{
	if(Orientation == SCREEN_HORIZONTAL_1)
	{
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_WIDTH,ILI9341_SCREEN_HEIGHT);
			
		HAL_GPIO_WritePin(GPIOB, DC_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi2, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
	}
	else if(Orientation == SCREEN_HORIZONTAL_2)
	{
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_WIDTH,ILI9341_SCREEN_HEIGHT);
			
		HAL_GPIO_WritePin(GPIOB, DC_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi2, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
	}
	else if(Orientation == SCREEN_VERTICAL_2)
	{
		ILI9341_Set_Rotation(SCREEN_VERTICAL_2);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_HEIGHT,ILI9341_SCREEN_WIDTH);
			
		HAL_GPIO_WritePin(GPIOB, DC_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi2, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
	}
	else if(Orientation == SCREEN_VERTICAL_1)
	{
		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_HEIGHT,ILI9341_SCREEN_WIDTH);
			
		HAL_GPIO_WritePin(GPIOB, DC_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi2, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
	}
}


