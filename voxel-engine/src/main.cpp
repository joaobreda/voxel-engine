#include "../headers/main.h"

const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
// last mouse pos
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
// delta time = time between current frame and last frame
float deltaTime = 0.0f, lastFrame = 0.0f;
// check if first time moving mouse to prevent mouse jump
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 270.0f, 0.0f));

int main() {
    GLFWwindow* window = setupWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel Engine");
    ChunkManager chunkManager;
    Shader shader("C:\\Users\\Breda\\Documents\\voxel-engine\\voxel-engine\\shaders\\vertex.glsl", "C:\\Users\\Breda\\Documents\\voxel-engine\\voxel-engine\\shaders\\fragment.glsl");

    shader.use();

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(55.0f/255.0f , 184.0f/255.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        // model matrix
        chunkManager.Render(shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}