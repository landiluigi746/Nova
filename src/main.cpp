#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <random>
#include <chrono>

int main()
{
	if (!glfwInit())
	{
        fmt::println("Failed to initialize GLFW!");
        return 1;
	}

	GLFWwindow* window;

	glfwSetErrorCallback([](int code, const char* msg) {
        fmt::println("GLFW error: {}, {}", code, msg);
    });

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "Nova Test", nullptr, nullptr);

	if (!window)
	{
        fmt::println("Failed to create GLFW window!");
        glfwTerminate();
        return 1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
        fmt::println("Failed to initialize GLAD!");
        return 1;
	}

	float r = 0.5f, g = 0.3f, b = 0.8f;

	std::default_random_engine randEngine(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
        glfwPollEvents();

		glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// EPILEPSY WARNING!
		r = dist(randEngine);
        g = dist(randEngine);
        b = dist(randEngine);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
