#include <stdlib.h>
#include <stdio.h>
#include <GL/glfw.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <emscripten/emscripten.h>

typedef void (*renderFunc)();

void doRenderingLoop(renderFunc doRendering) {
    emscripten_set_main_loop(doRendering, 0, 1);
}

float rotationX, rotationY, rotationZ;
GLuint textures[1];

void initTextures() {
    SDL_Surface *image;
	int flags = IMG_INIT_PNG;
	int initRes = IMG_Init(flags);
	if ((initRes & flags) != flags) {
		printf("Could not init specified image format: %s\n", IMG_GetError());
	}
    if(!(image = IMG_Load("opengl_demo/data/qafoo.png"))) {
        printf("Could not load texture image: %s\n", IMG_GetError());
        exit(-1);
    }

    glGenTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    printf("%ux%u (%u)\n", image->w, image->h, image->format->BytesPerPixel);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image);

    glEnable(GL_TEXTURE_2D);
}

void initGlScene() {
    printf("init GlScene\n");
    int sceneWidth, sceneHeight;

    initTextures();

    glClearColor(.41f, 0.71f, 0.4f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glfwGetWindowSize(&sceneWidth, &sceneHeight);
    gluPerspective(45.0f, (GLfloat)sceneWidth / (GLfloat)sceneHeight, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

void renderGlScene(double delta) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f,0.0f,-5.0f);
    
    glRotatef(rotationX,1.0f,0.0f,0.0f);
    glRotatef(rotationY,0.0f,1.0f,0.0f);
    glRotatef(rotationZ,0.0f,0.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glBegin(GL_QUADS);
    
    // Front Face (note that the texture's corners have to match the quad's corners)
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad

    // Back Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad

    // Top Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    
    // Bottom Face       
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    
    // Right face
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    
    // Left Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    
    glEnd();

    rotationX += delta * 25.0f;
    rotationY += delta * 25.0f;
    rotationZ += delta * 25.0f;
}

double lastSceneRendered, currentSceneRendered;

void renderFrame() {
    currentSceneRendered = glfwGetTime();
    renderGlScene(currentSceneRendered - lastSceneRendered);
    lastSceneRendered = currentSceneRendered;
    glfwSwapBuffers();
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        printf("Could not initialize library\n");
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    if(glfwOpenWindow(640, 480, 0,0,0,0,16,0, GLFW_WINDOW) != GL_TRUE) {
        printf("Could not create OpenGL window\n");
        glfwTerminate();
        return -1;
    }

    initGlScene();
    lastSceneRendered = glfwGetTime();
    doRenderingLoop(&renderFrame);

    glfwTerminate();
    return 0;
}
