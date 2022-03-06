#include "../headers/main.h"

const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;
// last mouse pos
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
// delta time = time between current frame and last frame
float deltaTime = 0.0f, lastFrame = 0.0f;
// check if first time moving mouse to prevent mouse jump
bool firstMouse = true;
Camera camera(glm::vec3(50.0f, 30.0f, 100.0f));

int main() {
    GLFWwindow* window = setupWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel Engine");
    ChunkManager chunkManager;
    Shader shader("C:\\Users\\Breda\\Documents\\voxel-engine\\voxel-engine\\shaders\\vertex.glsl", "C:\\Users\\Breda\\Documents\\voxel-engine\\voxel-engine\\shaders\\fragment.glsl");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    // imGUI state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    

    shader.use();

    while (!glfwWindowShouldClose(window)) {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2((float)350, (float)100));

        {
            ImGui::Begin("Debug");                          // Create a window called "Debug" and append into it.
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Camera pos. X: %.1f | Y: %.1f | Z: %.1f", camera.Position[0], camera.Position[1], camera.Position[2]);
            ImGui::End();
        }

        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        ImGui::Render();

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

        // Render imGUI after models for it to appear on top
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}