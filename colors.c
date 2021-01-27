#include "cube.h"

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

int		get_t(int trgb)
{
    printf("Transparency %x\n",trgb & (0xFF << 24));
	return (trgb & (0xFF << 24));
}

int		get_r(int trgb)
{
    printf("Red %x\n",trgb & (0xFF << 16));
	return (trgb & (0xFF << 16));
}

int		get_g(int trgb)
{
    printf("Green %x\n",trgb & (0xFF << 8));
	return (trgb & (0xFF << 8));
}

int		get_b(int trgb)
{
    printf("Blue %x\n",(trgb & 0xFF));
	return (trgb & 0xFF);
}
