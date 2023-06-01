#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<iostream>

//produces random integers b/w 0 and 1 for a maze of size mxm
int*** generateParents(int m, int population) {   
    int*** parents = new int** [population];   
    srand(time(0));
    for (int k = 0; k < population; k++) {
        parents[k] = new int* [m];
        for (int i = 0; i < m; i++) {
            parents[k][i] = new int[m];
            for (int j = 0; j < m; j++) {
                parents[k][i][j] = rand() % 2;
            }
        }
    }
    return parents;
}

//To set atleast 1 path through the maze, 
//returns the best pattern index based on longest path
int fix_chromosomes(int*** gen, int size, int population) {   
    int* dist = new int[population];
    for (int i = 0; i < population; i++) {
        dist[i] = 1;
    }
    
    for (int i = 0; i < population; i++) {     //row-wise iteration
        int x = 0, y = 0;
        int j = 0;
        int k = 0;        
        gen[i][j][k] = 1;
        int prev_k = 0;
        while (j < size) {                    //column-wise iteration for every row
        ss:
            if (j == size - 1) {              // condition last row
                if (prev_k < size / 2) {
                    while (k + 1 < size) {
                        gen[i][j][k + 1] = 1;
                        k++;
                        dist[i]++;
                    }
                    //dist[i]=dist[i]-1;
                    j++;
                    break;
                }
                else if (prev_k >= size / 2) {           
                    int t = k;
                    while (t < prev_k) {
                        gen[i][j - 1][t] = 0;
                        t++;
                        dist[i]--;
                    }
                    k = prev_k;
                    gen[i][j][k] = 1;
                    dist[i]++;

                    while (k + 1 < size) {
                        gen[i][j][k + 1] = 1;
                        k++;
                        dist[i]++;
                    }
                    //dist[i]=dist[i]-1;
                    j++;
                    break;
                }
            }
            else if (k < size / 2 && prev_k < size / 2) {         
                prev_k = k;
                while (k >= 0 && k < size) {
                    if (k == size - 1) {              //corner cell of the row
                        gen[i][j + 1][k] = 1;
                        dist[i]++;
                        j++;
                        break;
                    }
                    if (gen[i][j][k + 1] == 1) {
                        dist[i]++;
                        k++;
                        continue;
                    }
                    else if (gen[i][j + 1][k] != 1) {
                        gen[i][j][k + 1] = 1;
                        dist[i]++;
                        k++;
                        continue;
                    }
                    else if (gen[i][j + 1][k] == 1) {
                        dist[i]++;
                        j++;
                        break;
                    }
                }
            }
            else if (k >= size / 2 && prev_k < size / 2) {         //k>=size/2
                if (j == size - 2) {
                    j++;
                    while (k + 1 < size) {
                        gen[i][j][k + 1] = 1;
                        k++;
                        dist[i]++;
                    }
                    j++;
                    break;
                }

                prev_k = k;
                gen[i][j + 1][k] = 1;
                dist[i]++;
                j++;

                while (k >= 0 && k < size) {
                    if (k == 0) {              //corner cell of the row
                        gen[i][j + 1][k] = 1;
                        dist[i]++;
                        j++;
                        break;
                    }
                    if (gen[i][j][k - 1] == 1) {
                        dist[i]++;
                        k--;
                        continue;
                    }
                    else if (gen[i][j + 1][k] != 1) {
                        gen[i][j][k - 1] = 1;
                        dist[i]++;
                        k--;
                        continue;
                    }
                    else if (gen[i][j + 1][k] == 1) {
                        dist[i]++;
                        j++;
                        break;
                    }
                }
            }
            else if (k >= size / 2 && prev_k >= size / 2) {         //k>=size/2
                prev_k = k;
                while (k >= 0 && k < size) {
                    if (k == 0) {              //corner cell of the row
                        gen[i][j + 1][k] = 1;
                        dist[i]++;
                        j++;
                        break;
                    }
                    if (gen[i][j][k - 1] == 1) {
                        dist[i]++;
                        k--;
                        continue;
                    }
                    else if (gen[i][j + 1][k] != 1) {
                        gen[i][j][k - 1] = 1;
                        dist[i]++;
                        k--;
                        continue;
                    }
                    else if (gen[i][j + 1][k] == 1) {
                        dist[i]++;
                        j++;
                        break;
                    }
                }
            }
            else if (k < size / 2 && prev_k >= size / 2) {
                prev_k = k;
                gen[i][j + 1][k] = 1;
                dist[i]++;
                j++;
                if (j == size - 1) {
                    goto ss;
                }
                while (k >= 0 && k < size) {
                    if (k == size - 1) {              //corner cell of the row
                        gen[i][j + 1][k] = 1;
                        dist[i]++;
                        break;
                    }
                    if (gen[i][j][k + 1] == 1) {
                        dist[i]++;
                        k++;
                        continue;
                    }
                    else if (gen[i][j + 1][k] != 1) {
                        gen[i][j][k + 1] = 1;
                        dist[i]++;
                        k++;
                        continue;
                    }
                    else if (gen[i][j + 1][k] == 1) {
                        dist[i]++;
                        break;
                    }
                }
            }
        }
    }
    int best = 0;
    for (int i = 0; i < population; i++) {
        if (dist[best] < dist[i]) {
            best = i;
        }
    }
    return best ;
}

int population = 5;
int*** ga = generateParents(35, population);
int n = 35;
int best = fix_chromosomes(ga, n, population); //Select the 'best' maze pattern to dispaly 

int x, y;
int i;
int count;
int block_size = 5; 
int px = 0;                 //position of player 
int py = 0;                 //position of player 
int df = 10;                //to decide which screen to display
clock_t start;              //store the start time
clock_t end;                //store the time lapsed 
int k = 0;
int l = 0;

void point()                   //square for dynamic movements by the player 
{

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2i(px, py);
    glVertex2i(px + block_size, py);
    glVertex2i(px + block_size, py + block_size);
    glVertex2i(px, py + block_size);
    glEnd();
}
void point1()                //square depicting the starting position of the maze
{

    glColor3f(.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(0 + block_size, 0);
    glVertex2i(0 + block_size, 0 + block_size);
    glVertex2i(0, 0 + block_size);
    glEnd();
}
void point2()                   //square depicting the final winning position of the maze
{

    glColor3f(1.0, 0.0, .0);
    glBegin(GL_QUADS);
    glVertex2i(170, 170);
    glVertex2i(170 + block_size, 170);
    glVertex2i(170 + block_size, 170 + block_size);
    glVertex2i(170, 170 + block_size);
    glEnd();
}
void white_sq(int x, int y)         //to draw blocking squares
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + block_size, y);
    glVertex2i(x + block_size, y + block_size);
    glVertex2i(x, y + block_size);
    glEnd();
}

void output(int x, int y, const char* string)     //prints a string at (x,y) location
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        //displays character in a particular font style
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);    
    }
}

void drawstring(int x, int y, const char* string, void* font) //prints a string at (x,y) location in 'font' style
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        //displays character in a particular font style
        glutBitmapCharacter(font, string[i]);
    }
}

void frontscreen(void)          //First screen of the game 
{

    glClear(GL_COLOR_BUFFER_BIT);        //glClear(GL_COLOR_BUFFER_BIT) clears the OpenGL color buffers
    //The identity matrix, in terms of the projection and modelview matrices,resets the matrix back to its default state
    glLoadIdentity();         
    glColor3f(1, 1, 1);
    drawstring(120, 5, " Press ENTER to go To next screen", GLUT_BITMAP_HELVETICA_18);
    drawstring(-45, 5, "Maximize window for better view", GLUT_BITMAP_HELVETICA_12);
    glColor3f(1, 1, 1);
    output(10, 160, " THAPAR INSTITUTE OF ENGINEERING AND TECHNOLOGY");
    glColor3f(1, 1, 1);
    output(10.0, 150, "DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");
    glColor3f(1, 0, 1);
    output(60, 130, "A Mini Project On:-");
    glColor3f(0, 1, 0.5);
    output(60, 120, "\"BOX MAZE GAME\"");
    glColor3f(1, 0, 1);
    output(40, 100, "By :");
    glBegin(GL_LINES);
    glVertex2f(40, 98);
    glVertex2f(50, 98);
    glEnd();
    glColor3f(1, 0, 0);
    output(40, 90, "ARMAAN	           SEHAJ            	HIMMAT ");
    output(40, 80, "102003165	        102003173         102003330 ");

    output(40, 80, "");
    glColor3f(1, 0, 1);
    output(40, 60, "Under the Guidence of :");
    glBegin(GL_LINES);
    glVertex2f(40, 58);
    glVertex2f(98, 58);
    glEnd();
    glColor3f(1, 0, 0);
    output(40, 50, "");
    glColor3f(1, 0, 0);
    drawstring(72, 50, "(B.E.)", GLUT_BITMAP_HELVETICA_12);
    glColor3f(1, 0, 0);
    output(70, 40, "Dr. Anupam Garg");
    glColor3f(1, 0, 0);
    output(70, 30, "Assistant Professor,Dept. of CSE");
    glColor3f(1, 0, 0);
    output(40, 30, "");

    // Draw circles at random positions
    srand(time(NULL));      // Initialize random number generator with current time
    int radius = 2;
    int numCircles = 90;
    for (int i = 0; i < numCircles; i++) {
        // Generate random x and y coordinates within window bounds
        int x = rand() % 330;
        int y = rand() % 180;

        if (x <= 75 || x >= 240 && y>10) {
            glPushMatrix();                             // to save the current matrix state before performing the translations
            glTranslatef(x - 75, 0.0, 0.0); // Translate in x direction
            glTranslatef(0.0, y, 0.0); // Translate in y direction
            // Draw circle here
            glColor3f(rand() % 2, rand() % 2, rand() % 2);      //generates random color combination
            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j < 360; j++) {
                float theta = j * (2.0 * 3.14159265359 / 360.0);
                glVertex2f(radius * cos(theta), radius * sin(theta));
            }
            glEnd();
            glPopMatrix();         //restore the current matrix state  after performing the translations
        }
    }
    //empties all buffers, causing all issued commands to be executed as quickly as they are accepted by the actual rendering engine
    glFlush();

}

void winscreen()      // Gets displayed when the player reaches the final goal position
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0.0, 1.0, 0.0);
    output(55, 120, "CONGRATS!!!");
    glColor3f(1.0, 0.0, 1.0);
    output(15, 100, "YOU HAVE SUCCEEDED IN FINDING OUT THE PATH");
    output(35, 60, "* PRESS ESC TO GO TO MAIN MENU");
    output(35, 45, "* PRESS 1 TO RESTART THE GAME");
    
    //Display sparkles
    GLfloat M_PI = 3.14159265358979323846;            // Sparkles parameters
    GLfloat sparkleSize = 8.0;
    GLfloat sparkleSpeed = 0.8;

    GLfloat sparkles[50][6]; // sparkle positions and angles - 50 sparkles
    for (int i = 0; i < 50; i++) {
        // Set random initial positions
        sparkles[i][0] = (rand() % 330) - 75; // x position
        sparkles[i][1] = (rand() % 180) ; // y position
        sparkles[i][2] = 0.0; // z position (always 0)

        // Set random initial angles (in degrees)
        sparkles[i][3] = (GLfloat)(rand() % 360); // x angle
        sparkles[i][4] = (GLfloat)(rand() % 360); // y angle
        sparkles[i][5] = (GLfloat)(rand() % 360); // z angle
    }

    // Draw sparkles
    glPointSize(sparkleSize);
    glColor3f(rand() % 2, rand() % 2, rand() % 2);        //generates random color combination
    glBegin(GL_POINTS);
    for (int i = 0; i < 50; i++) {
        // Update sparkle positions and angles
        sparkles[i][0] += sparkleSpeed * sin(sparkles[i][3] * M_PI / 180.0); // x position
        sparkles[i][1] += sparkleSpeed * sin(sparkles[i][4] * M_PI / 180.0); // y position
        sparkles[i][2] += sparkleSpeed * cos(sparkles[i][5] * M_PI / 180.0); // z position
        sparkles[i][3] += (GLfloat)(rand() % 10); // x angle update
        sparkles[i][4] += (GLfloat)(rand() % 10); // y angle update
        sparkles[i][5] += (GLfloat)(rand() % 10); // z angle update

        glVertex3fv(sparkles[i]);                    //display sparkle point
    }
    glEnd();

    glFlush();
}
void startscreen()               // 2nd screen of the game
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);
    output(25, 140, "WELCOME TO THE GAME FINDING THE PATH");
    output(50, 100, "1.NEW GAME");
    output(50, 80, "2.INSTRUCTIONS");
    output(50, 60, "3.QUIT");
    glFlush();
}

void instructions()          //Instructions for the player
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 0.0);
    output(45, 140, "INSTRUCTIONS:");
    glBegin(GL_LINES);
    glVertex2f(45, 138);
    glVertex2f(90, 138);
    glEnd();
    glColor3f(0, 1, 0);
    output(-20, 120, "* TO MOVE THE POINT USE ARROW KEYS");
    output(-20, 100, "* FIND THE WAY TO MOVE INTO THE MAZE AND GET OUT");
    output(-20, 80, "* GREEN COLOURED POINT INDICATE THE POINT FROM WHERE YOU HAVE TO START");
    output(-20, 60, "* RED COLOURED POINT INDICATE THE POINT WHERE YOU HAVE TO REACH");
    output(-20, 40, "* YOU WILL HAVE TO HURRY AS YOU HAVE LIMITED TIME");
    output(-20, 20, "* PRESS ESC TO GO TO MAIN MENU");
    glFlush();
}

void timeover()           //gets displayed when the player fails to win in prescribed time
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0, 0);
    output(70, 110, "TIMEOVER");
    glColor3f(0, 1, 0);
    output(50, 100, "YOU HAVE LOST THE GAME");
    output(50, 90, "BETTER LUCK NEXT TIME");
    output(40, 40, "* PRESS ESC TO GO TO MAIN MENU");
    output(40, 30, "* PRESS 1 TO RESTART THE GAME");
    glFlush();
}
void display()                // displays the relevant screen according to user's choice and game results
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (df == 10)   
        frontscreen();                   //first screen

    else  if (df == 0)
        startscreen();                   //second screen

    else if (df == 1)                    //Game screen
    {
        output(-21, 0, "---->");
        output(179, 172, "---->");
        glColor3f(0.0, 0.0, 1.0);
        output(185, 160, "TOTAL TIME : 80 seconds");
        drawstring(190, 150, "HURRY UP", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1, 0, 0);
        drawstring(190, 140, "Time is running out", GLUT_BITMAP_HELVETICA_18);
        glutPostRedisplay();         //sets a flag so that on the next iteration of the mainloop, display() function is called.

        point();                     //depicts the player position
        point1();                    //square depicting the starting position of the maze
        point2();                    //square depicting the final(goal) position of the maze
        glutPostRedisplay();

        //Draw the maze
        int x = 0, y = 0;            
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (ga[best][i][j] == 1) {    // accessible path squares
                    //white_sq(x, y);
                }
                else {
                    white_sq(x, y);           //non- accessible path squares
                }
                y = y + block_size;
            }
            x = x + block_size;
            y = 0;
        }
        // Draw the outer border next
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(0, 0);
        glVertex2i(0 + 175, 0);
        glVertex2i(0 + 175, 0+175);
        glVertex2i(0, 0+175);
        glEnd();
        glutPostRedisplay();
    }

    else if (df == 2)     //Instructions' screen
        instructions();
    else if (df == 3)     //Quit the game
    {
        exit(1);
    }
    else if (df == 4)    //Player couldn't win
    {
        timeover();
    }
    else if (df == 5)    //Player wins
    {
        winscreen();
    }

    glFlush();

}
void idle()     // to set the timer of 80 seconds for the game
{
    if (df == 1)
    {
        end = clock();
        count = (end - start) / CLOCKS_PER_SEC;
        if (count == 80)                            
        {
            df = 4;                 //timeover
        }
        else
            if ((count < 80) && (px == 170) && (py == 170))        //winscreen
            {
                df = 5;
            }
    }

    glutPostRedisplay();
}
void SpecialKey(int key, int x, int y)   //monitor and control player movements
{
    switch (key)
    {
    case GLUT_KEY_UP:                  //UP movement
        if (py < 170) {
            if (ga[best][k][l + 1] == 1) {
                l++;
                py = py + block_size;
            }
        }
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:             //DOWN movement
        if (py > 0) {
            if (ga[best][k][l - 1] == 1) {
                l--;
                py = py - block_size;
            }
        }
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:            //LEFT movement
        if (px > 0) {
            if (ga[best][k - 1][l] == 1) {
                k--;
                px = px - block_size;
            }
        }
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:        //RIGHT movement
        if (px < 170) {
            if (ga[best][k + 1][l] == 1) {
                k++;
                px = px + block_size;
            }
        }
        glutPostRedisplay();
        break;
    }
}

//To toggle b/w different screens according to user's choice
void keyboard(unsigned char key, int x, int y)   
{

    if (df == 10 && key == 13)         //'Enter' key on first screen
        df = 0;                        //display second screen

    else if ((df == 0 || df == 4 || df == 5) && key == '1')          //display game screen ('1')
    {
        df = 1;                                          //for game screen
        start = clock();
        glutPostRedisplay();
    }
    else if (df == 0 && key == '2')              //instructions' screen ('2')
        df = 2;
    else if (df == 0 && key == '3')             //Quit game ('3')
        df = 3;
    else if (key == 27)          //Go to second screen on 'ESCAPE'
    {
        df = 0;
    }
    if ((key == '0' || key == '1') && (df == 4 || df == 1))  //reset the player position to start
    {
        px = 0;
        py = 0;
        l = 0;
        k = 0;
        //best = rand() % population;
    }
    glutPostRedisplay();
}
void myinit()
{
    glMatrixMode(GL_PROJECTION);   //defines the properties of the camera that 
                                   //views the objects in the world coordinate frame
    //The identity matrix, in terms of the projection and modelview matrices,sets the matrix to its default state
    glLoadIdentity();
    glPointSize(18.0);
    glMatrixMode(GL_MODELVIEW);    //defines how objects are transformed 
    glClearColor(0.0, 0.0, 0.0, 0.0);   //Set background color
}
void myreshape(int w, int h)          //reshapes the screen to fit for full screen display
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluOrtho2D(45.0, 135.0, -2.0 * (GLfloat)h / (GLfloat)w, 180.0 * (GLfloat)h / (GLfloat)w);
    else
        gluOrtho2D(-45.0 * (GLfloat)w / (GLfloat)h, 135.0 * (GLfloat)w / (GLfloat)h, -2.0, 180.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    //sets the initial display mode-GLUT_SINGLE:select a single buffered window,GLUT_RGB-shows color (Red, green, blue)
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize(600, 600);         //window dimensions
    glutCreateWindow("Maze game");        //Window name
    glutReshapeFunc(myreshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutSpecialFunc(SpecialKey);
    glutKeyboardFunc(keyboard);
    myinit();
    glutMainLoop();
    return 0;
}
