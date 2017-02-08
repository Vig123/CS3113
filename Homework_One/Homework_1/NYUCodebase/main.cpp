#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char* filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image.\n";
		assert(false);
	}

	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);
	return retTexture;
}


int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	// Setup
	glViewport(0, 0, 640, 360);
	//ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	GLuint playerTexture = LoadTexture(RESOURCE_FOLDER"player.png");
	GLuint bgTexture = LoadTexture(RESOURCE_FOLDER"bg.png");
	GLuint cloudTexture = LoadTexture(RESOURCE_FOLDER"cloud.png");
	GLuint sunTexture = LoadTexture(RESOURCE_FOLDER"sun.png");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

	float lastFrameTicks = 0.0f;
	glUseProgram(program.programID);


	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		// Game loop
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;


		glClearColor(0.118f, 0.565f, 1.0f, 1.0f); // color control
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, playerTexture);
		
		//modelMatrix.Translate(0.0f, 0.5f, 0.0f);
		//float scaleSize = 0.5f;
		//if (scaleSize < 1.0f) {
		//	scaleSize += lastFrameTicks;
		//}

		//modelMatrix.Scale(scaleSize, scaleSize, 1.0f);
		//modelMatrix.Rotate(90 * lastFrameTicks * 3.14159265 / 180);

		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);

		modelMatrix.identity();
		modelMatrix.Translate(0.0f, 0.25f, 0.0f);
		program.setModelMatrix(modelMatrix);

		
		//float vertices[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
		float vertices[] = { -0.5f, -0.75f, 0.5f, -0.75f, 0.5f, 0.75f, -0.5f, -0.75f, 0.5f, 0.75f, -0.5f, 0.75f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		
		modelMatrix.identity();
		program.setModelMatrix(modelMatrix);

		glBindTexture(GL_TEXTURE_2D, bgTexture);

		float bgVertices[] = { 3.55f, -0.5f, -3.55f, -0.5f, -3.55f, -2.0f, -3.55f, -2.0f, 3.55f, -2.0f, 3.55f, -0.5f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, bgVertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float bgTexCoords[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, bgTexCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		glBindTexture(GL_TEXTURE_2D, cloudTexture);

		float cloudVertices[] = { -1.0f, 1.8f, -3.4f, 1.8f, -3.4f, 1.0f, -3.4f, 1.0f, -1.0f, 1.0f, -1.0f, 1.8f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, cloudVertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float cloudTexCoords[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, cloudTexCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);


		modelMatrix.identity();
		modelMatrix.Translate(2.3f, 0.9f, 0.0f);
		modelMatrix.Rotate(45 * lastFrameTicks * 3.14159265 / 180);
		program.setModelMatrix(modelMatrix);

		glBindTexture(GL_TEXTURE_2D, sunTexture);

		float sunVertices[] = { 0.8f, 0.8f, -0.8f, 0.8f, -0.8f, -0.8f, -0.8f, -0.8f, 0.8f, -0.8f, 0.8f, 0.8f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, sunVertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float sunTexCoords[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, sunTexCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
