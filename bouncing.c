// Compile with: gcc final.c -o final -lGL -lGLU -lglut -lm
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#define Lx 20
#define Ly 20
#define NUM_BALLS 9
#define SIDES_BALLS 30
#define R_BALLS 1
#define PI 3.14
#define g 0.01

float 
	ball_x[NUM_BALLS], 
	ball_y[NUM_BALLS], 
	ball_vx[NUM_BALLS],
	ball_vy[NUM_BALLS],
	X0 = -Lx/2,
	X1 = 0,
	X2 = Lx/2,
	Y0 = Ly/2,
	Y1 = -Ly/2,
	Y2 = Ly/2;

void Initialize(void);
void display(void);
void draw_arena(void);
void draw_ball(float, float, int);
void animate_ball(void);
void key(unsigned char, int, int);
void Timer (int);

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Final");
    glutDisplayFunc(display);  // display function is display
	glutKeyboardFunc(key);     // keystroke interpreter is key
    Initialize();
	Timer(0);
    glutMainLoop();
    return 0;
}

void Initialize(void){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-Lx, Lx, -Ly, Ly);
		for (int k=0; k<NUM_BALLS; k++){
			ball_x[k] = -1*k-1;
			ball_y[k] = -2*ball_x[k] - 10;
			ball_vx[k] = sqrt(-ball_x[k]*g);
			ball_vy[k] = ball_vx[k];
		}
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
	draw_arena();
	for (int k=0; k<NUM_BALLS; k++){
		draw_ball(ball_x[k], ball_y[k], k);
		if (k < NUM_BALLS-1){
			glBegin(GL_LINES);
				glVertex2f(ball_x[k], ball_y[k]);
				glVertex2f(ball_x[k+1], ball_y[k+1]);
			glEnd();
		}
	}
//		draw_ball(ball_x[NUM_BALLS], ball_y[NUM_BALLS], NUM_BALLS);
    glutSwapBuffers();
	animate_ball();
}

void draw_arena(void){
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);

		glVertex2f(-10, 10);
		glVertex2f(0, -10);
		glVertex2f(0, -10);
	  glVertex2f(10, 10);
	
	glEnd();             
}


void draw_ball(float x, float y, int a){ // the thing moving around
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
/*
    glBegin(GL_POINTS);
        glVertex2f(x,y);
    glEnd();
*/
// for circular balls
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(5.0);
		glBegin(GL_LINES); // make the colors a gradient from blue to red
		for (int i=0; i<= NUM_BALLS; i++){
			glColor4f(0.9-a*0.1, 0.0+a*0.1, 0.0+a*0.1, 1.0);
		}
			for(int i = 0; i <= SIDES_BALLS; i++) {
				glVertex2f(x,y);
				glVertex2f( x+R_BALLS*cos(2*PI*i/SIDES_BALLS), y+R_BALLS*sin(2*PI*i/SIDES_BALLS) );
			}
		glEnd();

}

void animate_ball(void){
	for (int k=0; k<NUM_BALLS; k++){

		// add a dt

		ball_x[k] += ball_vx[k];
		ball_y[k] += ball_vy[k]; 
 	  ball_vy[k] = ball_vy[k] - g;

	// we need equation of the two lines we are comparing. let's split into two (left and right side)
/*
		left side: y-int = -10
		x = -10 when y = 10. x = 0 when y = -10. so the equation: y = -2x - 10, -0.5y - 5 = x
		right side: y-int = -10
		x = 10 when y = 10. so the equation: y = 2x - 10, 0.5y + 5 = x
*/

		if (ball_x[k] < (-0.5*ball_y[k] - 5)){ 
			ball_x[k] = -1*k-1;
			ball_y[k] = -2*ball_x[k] - 10;
			ball_vx[k] = sqrt(-ball_x[k]*g);
			ball_vy[k] = ball_vx[k];
		} else if (ball_x[k] > (0.5*ball_y[k] + 5)){
			ball_x[k] = 1*k+1;
			ball_y[k] = 2*ball_x[k] - 10;
			ball_vx[k] = -sqrt(ball_x[k]*g);
			ball_vy[k] = -ball_vx[k];
		}

/*		if ((ball_y[k] < (-2*ball_x[k] - 10) && ball_x[k] <= 0) || (ball_y[k] > (2*ball_x[k] - 10) && ball_x[k] > 0)){
			ball_vy[k] =-ball_vy[k];
		} */
	}
}

void key(unsigned char key, int x, int y){
	switch(key) {
	case 'q': exit(0);
	default: break;
	}
}

void Timer (int iUnused){
	glutPostRedisplay();
	glutTimerFunc(5, Timer, 0);
}
