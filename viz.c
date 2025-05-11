#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void panic_errno(const char* fmt, ...) {

	fprintf(stderr, "ERROR: ");

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, ": %s\n", strerror(errno));

	exit(1);
}

const char* slurp_file(const char* file_path) {
#define SLURP_FILE_PANIC panic_errno("Could not read file `%s`", file_path);
	FILE* f = fopen(file_path, "r");
	if (f == NULL)
		SLURP_FILE_PANIC;
	if (fseek(f, 0, SEEK_END) < 0)
		SLURP_FILE_PANIC;

	long size = ftell(f);
	if (size < 0)
		SLURP_FILE_PANIC;

	char* buffer = malloc(size + 1);
	if (buffer == NULL)
		SLURP_FILE_PANIC;

	if (fseek(f, 0, SEEK_SET) < 0)
		SLURP_FILE_PANIC;

	fread(buffer, 1, size, f);

	if (ferror(f) < 0)
		SLURP_FILE_PANIC;

	buffer[size] = '\0';
	if (fclose(f) < 0)
		SLURP_FILE_PANIC;

	return buffer;
#undef SLURP_FILE_PANIC
}

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "Error: could not initialize GLFW\n");
		exit(1);
	}

	GLFWwindow* const window = glfwCreateWindow(
		SCREEN_WIDTH, SCREEN_HEIGHT, "Cellular Automata", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Error: could not create a window\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
