#include "include/imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"
#include "include/glfw3.h"
#include "include/ktx.h"
#include <string>
#include <iostream>

class KTXLoader {
public:
    KTXLoader() : m_TextureID(0) {}

    ~KTXLoader() {
        if (m_TextureID != 0) {
            glDeleteTextures(1, &m_TextureID);
        }
    }

    bool LoadKTX(const char* filename) {
        ktxTexture* kTexture;
        KTX_error_code result;

        result = ktxTexture_CreateFromNamedFile(filename, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &kTexture);
        if (result != KTX_SUCCESS) {
            std::cerr << "Failed to load KTX file: " << filename << std::endl;
            return false;
        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        // Corrected ktxTexture_GLUpload call
        GLenum target;
        GLenum error;
        result = ktxTexture_GLUpload(kTexture, &m_TextureID, &target, &error);
        if (result != KTX_SUCCESS) {
            std::cerr << "Failed to upload KTX texture to OpenGL. Error: " << error << std::endl;
            ktxTexture_Destroy(kTexture);
            return false;
        }

        ktxTexture_Destroy(kTexture);
        return true;
    }

    void RenderKTX(float x = ImGui::GetWindowSize().y * 0.1, float y = ImGui::GetWindowSize().y * 0.1) {
        if (m_TextureID != 0)
            ImGui::Image((void*)(intptr_t)m_TextureID, ImVec2(x, y));
    }

private:
    GLuint m_TextureID;
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
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui KTXLoader Demo", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Enable VSync
    glfwSwapInterval(1);

    // Loading KTX
    KTXLoader ktxLoader;
    bool ktxLoaded = false;
    std::string ktxPath;

    if (argc > 1) {
        ktxPath = argv[1];
        ktxLoaded = ktxLoader.LoadKTX(ktxPath.c_str());
        if (!ktxLoaded) {
            std::cerr << "Failed to load KTX: " << ktxPath << std::endl;
        }
    }

    while (!ktxLoaded) {
        std::cout << "Please enter the path to a valid .ktx file: ";
        std::getline(std::cin, ktxPath);
        ktxLoaded = ktxLoader.LoadKTX(ktxPath.c_str());
        if (!ktxLoaded) {
            std::cerr << "Failed to load KTX: " << ktxPath << std::endl;
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

        // ImGui KTX window
        ImGui::Begin("KTX Render");
        ImGui::SliderFloat("Icon Size", &size, ImGui::GetWindowSize().y * 0.1, 512);
        ktxLoader.RenderKTX(size, size);
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