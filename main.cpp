#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>
#include<mmsystem.h>
#include<math.h>

static int slices = 16;
static int stacks = 16;

const int mazeSize = 20; // Size of the maze (adjust as needed)
const float wallSize = 1.0f; // Size of each wall cube

int maze[mazeSize][mazeSize][mazeSize] = {
    {
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1},
        {1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    }
};
int lastMouseX, lastMouseY;
float rotateX = 0.0f;
float rotateY = 0.0f;
float zoom = 10.0f;
float zoomLevel = 10.0f;

int playerX = 1;  // Initial player position in the maze
int playerY = 1;
int playerZ = 1;
bool gameCompleted = false;

static void resize(int width, int height)
{
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, ar, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1, 0, 1);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -zoomLevel);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glTranslatef(-mazeSize / 2.0f, -mazeSize / 2.0f, -mazeSize / 2.0f);

    for (int x = 0; x < mazeSize; ++x)
    {
        for (int y = 0; y < mazeSize; ++y)
        {
            for (int z = 0; z < mazeSize; ++z)
            {
                if (maze[x][y][z] == 1)
                {
                    glPushMatrix();
                    glTranslatef(x * wallSize, y * wallSize, z * wallSize);
                    glutSolidCube(wallSize);
                    glPopMatrix();
                }
            }
        }
    }

    if (!gameCompleted)
    {
        // Render the player cube
        glColor3d(1, 0, 0); // Set player color to red
        glPushMatrix();
        glTranslatef(playerX * wallSize, playerY * wallSize, playerZ * wallSize);
        glutSolidCube(wallSize);
        glPopMatrix();
    }
    else
    {
        // Game completed, display a congratulations message
        glColor3d(0, 1, 0); // Set text color to green
        glRasterPos3f((mazeSize / 2) * wallSize, (mazeSize / 2) * wallSize, mazeSize * wallSize);
        char message[] = "Congratulations!";
        for (int i = 0; i < strlen(message); i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    }

    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    int newPlayerX = playerX;
    int newPlayerY = playerY;
    int newPlayerZ = playerZ;

    switch (key)
    {
        case 27:
        case 'q':
            exit(0);
            break;

        case 's':  // Move player to the left
            newPlayerY--;
            break;

        case 'w':  // Move player to the right
            newPlayerY++;
            break;

        case 'a':  // Move player upward
            newPlayerZ++;
            break;

        case 'd':  // Move player downward
            newPlayerZ--;
            break;
    }

    // Check if the new player position is within the maze box
    if (newPlayerX > 0 && newPlayerX < mazeSize - 1 &&
        newPlayerY > 0 && newPlayerY < mazeSize - 1 &&
        newPlayerZ > 0 && newPlayerZ < mazeSize - 1 &&
        maze[newPlayerX][newPlayerY][newPlayerZ] == 0)
    {
        playerX = newPlayerX;
        playerY = newPlayerY;
        playerZ = newPlayerZ;
    }

    // Limit player's movement within the maze box
    if (playerX <= 0)
        playerX = 1;
    else if (playerX >= mazeSize - 1)
        playerX = mazeSize - 2;

    if (playerY <= 0)
        playerY = 1;
    else if (playerY >= mazeSize - 1)
        playerY = mazeSize - 2;

    if (playerZ <= 0)
        playerZ = 1;
    else if (playerZ >= mazeSize - 1)
        playerZ = mazeSize - 2;

    glutPostRedisplay();
}


static void idle(void)
{
    glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        lastMouseX = x;
        lastMouseY = y;
    }

    // Handle mouse scroll events for zooming
    if (button == 3)  // Scroll Up
    {
        zoomLevel -= 1.0f;
    }
    else if (button == 4)  // Scroll Down
    {
        zoomLevel += 1.0f;
    }
}

static void motion(int x, int y)
{
    rotateX += (y - lastMouseY);
    rotateY += (x - lastMouseX);
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("3D Maze");
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH_TEST);
    PlaySound("song.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);

    glutMainLoop();

    return EXIT_SUCCESS;
}
