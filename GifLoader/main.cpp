#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"
#include "include/glfw3.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class GifLoader {
public:
    GifLoader() : m_CurrentFrame(0), m_TotalFrames(0), m_FrameDelay(0.1f), m_LastFrameTime(0.0f) {}

    ~GifLoader() {
        for (auto& texture : m_Frames) {
            GLuint textureID = static_cast<GLuint>(reinterpret_cast<intptr_t>(texture));
            glDeleteTextures(1, &textureID);
        }
    }

    bool LoadGif(const char* filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) {
            std::cerr << "Failed to read file: " << filename << std::endl;
            return false;
        }

        int* delays = nullptr;
        int x, y, z, comp, req_comp = 4;
        stbi_uc* gif_data = stbi_load_gif_from_memory(reinterpret_cast<stbi_uc*>(buffer.data()), size, &delays, &x, &y, &z, &comp, req_comp);

        if (!gif_data) {
            std::cerr << "Failed to decode GIF: " << filename << std::endl;
            std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
            return false;
        }

        m_TotalFrames = z;
        m_FrameDelay = delays[0] / 1000.0f;

        for (int i = 0; i < m_TotalFrames; ++i) {
            GLuint textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, gif_data + i * x * y * req_comp);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            m_Frames.push_back(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(textureID)));
        }

        stbi_image_free(gif_data);
        STBI_FREE(delays);
        return true;
    }

    void UpdateFrame() {
        float currentTime = ImGui::GetTime();
        if (currentTime - m_LastFrameTime >= m_FrameDelay) {
            m_CurrentFrame = (m_CurrentFrame + 1) % m_TotalFrames;
            m_LastFrameTime = currentTime;
        }
    }

    void RenderFrame(float x = ImGui::GetWindowSize().y * 0.1, float y = ImGui::GetWindowSize().y * 0.1) {
        if (!m_Frames.empty())
            ImGui::Image(m_Frames[m_CurrentFrame], ImVec2(x, y));
    }

private:
    std::vector<ImTextureID> m_Frames;
    int m_CurrentFrame;
    int m_TotalFrames;
    float m_FrameDelay;
    float m_LastFrameTime;
};

int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui GifLoader Demo", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Enable VSync
    glfwSwapInterval(1);

    // Loading The Gif

    GifLoader gifLoader;
    bool gifLoaded = false;
    std::string gifPath;

    if (argc > 1) {
        gifPath = argv[1];
        gifLoaded = gifLoader.LoadGif(gifPath.c_str());
        if (!gifLoaded) {
            std::cerr << "Failed to load gif: " << gifPath << std::endl;
        }
    }

    while (!gifLoaded) {
        std::cout << "Please enter the path to a valid .gif file: ";
        std::getline(std::cin, gifPath);
        gifLoaded = gifLoader.LoadGif(gifPath.c_str());
        if (!gifLoaded) {
            std::cerr << "Failed to load gif: " << gifPath << std::endl;
        }
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float size = 150;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // ImGui initialization
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui Gif window
        ImGui::Begin("Gif Render");
        gifLoader.UpdateFrame();
        ImGui::SliderFloat("Icon Size", &size, ImGui::GetWindowSize().y * 0.1, 512);
        gifLoader.RenderFrame(size, size);
        ImGui::End();

        // Rendering ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}