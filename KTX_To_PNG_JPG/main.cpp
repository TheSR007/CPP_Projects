#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/glfw3.h"
#include "include/ktx.h"
#include "include/stb_image_write.h"
#include <iostream>
#include <filesystem>

// Function to load KTX/KTX2 file and create OpenGL texture
GLuint LoadKTXTexture(const char* filename) {
    ktxTexture* kTexture;
    KTX_error_code result;
    GLuint textureID;

    result = ktxTexture_CreateFromNamedFile(filename, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &kTexture);
    if (result != KTX_SUCCESS) {
        std::cerr << "Failed to load KTX file: " << filename << std::endl;
        return 0;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Upload texture to OpenGL
    GLenum target;
    GLenum error;
    result = ktxTexture_GLUpload(kTexture, &textureID, &target, &error);
    if (result != KTX_SUCCESS) {
        std::cerr << "Failed to upload KTX texture to OpenGL. Error: " << error << std::endl;
        ktxTexture_Destroy(kTexture);
        return 0;
    }

    ktxTexture_Destroy(kTexture);
    return textureID;
}

// Function to convert KTX texture to PNG and JPEG
void ConvertKTXToPNGAndJPEG(GLuint textureID, const std::filesystem::path& filepath) {
    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    unsigned char* pixels = new unsigned char[width * height * 4];
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Save as PNG
    std::filesystem::path pngFilename = filepath.parent_path().parent_path() / "png" / (filepath.stem().string() + ".png");
    stbi_write_png(pngFilename.string().c_str(), width, height, 4, pixels, 0);

    // Save as JPEG
    std::filesystem::path jpegFilename = filepath.parent_path().parent_path() / "jpg" / (filepath.stem().string() + ".jpg");
    stbi_write_jpg(jpegFilename.string().c_str(), width, height, 4, pixels, 100);

    delete[] pixels;
}

int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // Create a hidden window to initialize OpenGL context
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hidden", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    std::filesystem::path imagesDir;
    bool validDirectory = false;

    if (argc > 1) {
        imagesDir = std::filesystem::path(argv[1]);
    } else {
        std::string userInput;
        std::cout << "Enter the directory containing KTX files: ";
        std::getline(std::cin, userInput);
        imagesDir = std::filesystem::path(userInput);
    }

    // Check for command-line argument or prompt user
    do {
        if (std::filesystem::exists(imagesDir) && std::filesystem::is_directory(imagesDir)) {
            auto it = std::filesystem::directory_iterator(imagesDir);
            if (it != std::filesystem::end(it)) {
                for (const auto& entry : std::filesystem::directory_iterator(imagesDir)) {
                    if (entry.is_regular_file() && entry.path().extension() == ".ktx") {
                        validDirectory = true;
                        break;
                    }
                }
                if (!validDirectory) {
                    std::cerr << "Directory contains no .ktx files. Please provide a valid directory." << std::endl;
                    imagesDir.clear();
                }else {
                    break;
                }
            } else {
                std::cerr << "Directory is empty. Please provide a valid directory." << std::endl;
                imagesDir.clear();
            }
        } else {
            std::cerr << "Directory does not exist. Please provide a valid directory." << std::endl;
            imagesDir.clear();
        }

        // Ask for directory
        std::string userInput;
        std::cout << "Enter the directory containing KTX files: ";
        std::getline(std::cin, userInput);
        imagesDir = std::filesystem::path(userInput);
    } while (!validDirectory);

    std::filesystem::create_directory(imagesDir.parent_path() / "png");
    std::filesystem::create_directory(imagesDir.parent_path() / "jpg");

    // Iterate over KTX files in directory and convert them
    for (const auto& entry : std::filesystem::directory_iterator(imagesDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".ktx") {
            GLuint ktxTextureID = LoadKTXTexture(entry.path().string().c_str());
            if (ktxTextureID != 0) {
                ConvertKTXToPNGAndJPEG(ktxTextureID, entry.path());
                glDeleteTextures(1, &ktxTextureID);
            }
        }
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
