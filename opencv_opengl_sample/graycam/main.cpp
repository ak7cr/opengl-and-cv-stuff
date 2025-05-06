#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    // Select camera index (default = 0)
    int camIndex = 0;
    if (argc > 1) camIndex = std::atoi(argv[1]);

    // Open the camera
    cv::VideoCapture cap(camIndex);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera #" << camIndex << std::endl;
        return -1;
    }

    // Set desired resolution
    int width = 640, height = 480;
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    // Determine FPS for writer
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0.0) fps = 30.0;

    // Ensure output directory exists
    std::filesystem::path outDir = "output";
    if (!std::filesystem::exists(outDir)) {
        std::filesystem::create_directories(outDir);
    }

    // Initialize video writer to save grayscale output in output folder
    std::string outPath = (outDir / "output.mp4").string();
    cv::VideoWriter writer(
        outPath,
        cv::VideoWriter::fourcc('m','p','4','v'),
        fps,
        cv::Size(width, height),
        false
    );
    if (!writer.isOpened()) {
        std::cerr << "Error: Could not open video writer at " << outPath << std::endl;
        return -1;
    }

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Error: Failed to initialize GLFW" << std::endl;
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(width, height, "Grayscale Video (OpenCV + OpenGL)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Create OpenGL texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Main capture-render loop
    while (!glfwWindowShouldClose(window)) {
        cv::Mat frame;
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "Error: Failed to capture frame" << std::endl;
            break;
        }

        // Convert to grayscale
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // ───────────────────────────────────────────────────────────
        // 1) Horizontal flip for saved video
        cv::Mat horizFlip;
        cv::flip(gray, horizFlip, 1); // 1 = horizontal
        writer.write(horizFlip);

        // 2) Vertical flip for OpenGL (to correct origin)
        cv::Mat flipped;
        cv::flip(gray, flipped, 0);   // 0 = vertical
        // ───────────────────────────────────────────────────────────

        // Upload to OpenGL texture (luminance)
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, flipped.cols, flipped.rows,
                     0, GL_LUMINANCE, GL_UNSIGNED_BYTE, flipped.data);

        // Render textured quad
        int fbw, fbh;
        glfwGetFramebufferSize(window, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Exit on ESC key press
        if (cv::waitKey(1) == 27) break;
    }

    // Cleanup
    writer.release();
    glDeleteTextures(1, &texture);
    cap.release();
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Saved horizontally-flipped grayscale video to " << outPath << std::endl;
    return 0;
}


// Compile with:
// g++ -o cvgl cvgl.cpp -lGL -lGLEW -lglfw -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
// Run with:
// ./cvgl <camera_index>
// Example: ./cvgl 0
// Note: Make sure to have OpenGL, GLEW, GLFW, and OpenCV installed and properly linked.
// This code captures video from a camera, converts it to grayscale, and displays it using OpenGL.
// It also saves the grayscale video to a file named "output.avi" in the current directory.
// The code uses OpenCV for video capture and processing, and OpenGL for rendering.
// The program will exit when the ESC key is pressed in the OpenCV window or when the camera feed ends.
// The code is designed to be cross-platform and should work on Linux, macOS, and Windows with the appropriate libraries installed.
// The code is a simple example of integrating OpenCV with OpenGL for real-time video processing and rendering.