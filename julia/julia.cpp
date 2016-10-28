// julia.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#define DIM 1000 /* rozmiar rysunku w pikselach */
int julia(float x, float y)
{
	const int k = 500;
	const float ci = 0.745;
	const float cr = -0.123;
	int R2 = 4;
	int i = 0;
	int x2 = x*x;
	int y2 = y*y;
	while (i < k) {
		if (x2 + y2 > R2) {
			return 0;
		}
		y = 2 * x*y + ci;
		x = x2 - y2 + cr;
		i = i + 1;
		x2 = x * x;
		y2 = y * y;
	}
	return 1;
}


void kernel(unsigned char *ptr,
	const int xw, const int yw,
	const float dx, const float dy,
	const float scale)
{
	/* przeliczenie wspó³rzêdnych pikselowych (xw, yw)
	na matematyczne (x, y) z uwzglêdnieniem skali
	(scale) i matematycznego œrodka (dx, dy) */
	float x = scale*(float)(xw - DIM / 2) / (DIM / 2) + dx,
		y = scale*(float)(yw - DIM / 2) / (DIM / 2) + dy;
	int offset /* w buforze pikseli */ = xw + yw*DIM;
	/* kolor: czarny dla uciekinierów (julia == 0)
	czerwony dla wiêŸniów (julia == 1) */
	ptr[offset * 4 + 0 /* R */] = (unsigned char)(255 * julia(x, y));
	ptr[offset * 4 + 1 /* G */] = 0;
	ptr[offset * 4 + 2 /* B */] = 0;
	ptr[offset * 4 + 3 /* A */] = 255;
}
/**************************************************/

#define WIN32
#include "GL/freeglut.h"
static unsigned char pixbuf[DIM * DIM * 4];
static float dx = 0.0f, dy = 0.0f;
static float scale = 1.5f;
static void disp(void)
{
	glDrawPixels(DIM, DIM, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf);
	glutSwapBuffers();
}
static void recompute(void)
{
	int xw, yw;
	for (yw = 0; yw < DIM; yw++)
		for (xw = 0; xw < DIM; xw++)
			kernel(pixbuf, xw, yw, dx, dy, scale);
	glutPostRedisplay();
}
static void kbd(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p': dx += scale*(float)(x - DIM / 2) / (DIM / 2);
		dy -= scale*(float)(y - DIM / 2) / (DIM / 2);
		break;
	case 'z': scale *= 0.80f; break;
	case 'Z': scale *= 1.25f; break;
	case '=': scale = 1.50f; dx = dy = 0.0f; break;
	case 27: /* Esc */ exit(0);
	}
	recompute();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv); /* inicjacja biblioteki GLUT */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /* opcje */
	glutInitWindowSize(DIM, DIM); /* rozmiar okna graficznego */
	glutCreateWindow("RIM - fraktal Julii"); /* tytu³ okna */
	glutDisplayFunc(disp); /* funkcja zwrotna zobrazowania */
	glutKeyboardFunc(kbd); /* funkcja zwrotna klawiatury */
	recompute(); /* obliczenie pierwszego rysunku */
	glutMainLoop(); /* g³ówna pêtla obs³ugi zdarzeñ */
}