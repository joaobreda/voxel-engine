#include "../headers/main.h"

const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;
// Last mouse pos
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
// Delta time = time between current frame and last frame
float deltaTime = 0.0f, lastFrame = 0.0f;
// Check if first time moving mouse to prevent mouse jump
bool firstMouse = true;
Camera camera(glm::vec3(50.0f, 70.0f, 100.0f));

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
    static bool wireframeCheckBox = false;
    float frustumFarBoundary = 300.0f;
    
    shader.use();

    while (!glfwWindowShouldClose(window)) {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2((float)320, (float)210));
        // Creating an imGUI window
        {
            ImGui::Begin("Debug");
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Press T to capture/uncapture mouse");
            ImGui::Text("Avg FPS: %.2f", ImGui::GetIO().Framerate);
            ImGui::Text("Camera pos. X: %.1f | Y: %.1f | Z: %.1f", camera.Position[0], camera.Position[1], camera.Position[2]);
            ImGui::Text("Camera world pos. X: %.1f | Y: %.1f | Z: %.1f", floor(camera.Position[0] / CX), floor(camera.Position[1] / CY), floor(camera.Position[2] / CZ));
            ImGui::Text("Blocks per chunk: %dx%dx%d", CX, CY, CZ);
            ImGui::Text("World chunks: %dx%dx%d", SCX, SCY, SCZ);
            ImGui::Checkbox("Wireframe", &wireframeCheckBox);
            ImGui::Text("Frustum far boundary: ");
            ImGui::SliderFloat("float", &frustumFarBoundary, 0.1f, 5000.0f);

            ImGui::End();
        }

        // Render with wireframe or without?
        glPolygonMode(GL_FRONT_AND_BACK, wireframeCheckBox ? GL_LINE : GL_FILL);

        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Render
        ImGui::Render();

        glClearColor(55.0f/255.0f , 184.0f/255.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pass projection matrix to shader (note that in this case it could change every frame)
        // These last two parameters specify the lower and farest boundaries of our frustrum culling
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, frustumFarBoundary);
        shader.setMat4("projection", projection);

        // Camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        // Model matrix
        chunkManager.Render(shader);

        // Render imGUI after for it to appear on top of everything else
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}