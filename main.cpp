#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <Windows.h>
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/freeglut.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

void drawCube(float orientation[3], float position[3], float scale[3],
    float tone)
{
    // vertices
    GLfloat vertices[] = {
        -1, -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1, 1, -1, -1, 1, -1, 1,
        1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, -1, -1,
        -1, 1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, 1, -1,
        1, 1, -1, 1, -1, -1, -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, 1
    };

    // Define color gray
    GLfloat colors[72];
    for (int i = 0; i < 72; i++) {
        colors[i] = tone;
    }

    glPushMatrix();
    glRotatef(orientation[0], 1, 0, 0);
    glRotatef(orientation[1], 0, 1, 0);
    glRotatef(orientation[2], 0, 0, 1);
    glTranslatef(position[0], position[1], position[2]);
    glScalef(scale[0], scale[1], scale[2]);

    // We have a color array and a vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    // Send data : 24 vertices
    glDrawArrays(GL_QUADS, 0, 24);

    // Cleanup states
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void drawSphere(int lats, int longs, GLfloat x, GLfloat y, GLfloat z)
{
    glPushMatrix();
    glTranslatef(x, y + (GLfloat)0.15, z);
    glScalef((GLfloat)0.47, (GLfloat)0.47, (GLfloat)0.47);

    int i, j;

    for (i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glColor3f((GLfloat)1, (GLfloat)0.4, (GLfloat)0.1);
        glBegin(GL_QUAD_STRIP);

        for (j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double)(j - 1) / longs;
            double x2 = cos(lng);
            double y2 = sin(lng);

            glNormal3f((GLfloat)(x2 * zr0), (GLfloat)(y2 * zr0), (GLfloat)z0);
            glVertex3f((GLfloat)(x2 * zr0), (GLfloat)(y2 * zr0), (GLfloat)z0);
            glNormal3f((GLfloat)(x2 * zr1), (GLfloat)(y2 * zr1), (GLfloat)z1);
            glVertex3f((GLfloat)(x2 * zr1), (GLfloat)(y2 * zr1), (GLfloat)z1);
        }
        glEnd();
    }
    glPopMatrix();
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpComand,
    int nShow)
{
    HWND hWnd = GetConsoleWindow(); // MER  [Sep 20, 2013] This hides the window
    if (IsWindow(hWnd))
        ShowWindow(hWnd, SW_HIDE);
#else
int main(int, char**)
{
#endif

    float rotateCube = 0; // Apply rotation on cube

    // Setup window
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL2 3D example", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Setup ImGui binding
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_test_window = true;
    bool show_another_window = false;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Example")) {
                ImGui::MenuItem("Sub", "OK");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears
        // in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window"))
                show_test_window ^= 1;
            if (ImGui::Button("Another Window"))
                show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window) {
            ImGui::SetNextWindowSize(ImVec2(200, 100), 0);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in
        // ImGui::ShowTestWindow()
        if (show_test_window) {
            ImGui::SetNextWindowPos(ImVec2(650, 20), 0);
            ImGui::ShowDemoWindow(&show_test_window);
        }

        // Rendering
        int display_w, display_h;
        glfwGetWindowSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        /* Here we add some code to add the 3d world */
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            gluPerspective(60, (double)display_w / (double)display_h, 0.1, 3000);

            glMatrixMode(GL_MODELVIEW);

            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();

            // Positions the "camera"
            // Both respectively X, Y and Z
            int orientation[3] = { 60, 0, 0 };
            int position[3] = { 0, -20, -15 };

            glRotatef((GLfloat)orientation[0], (GLfloat)1, 0, 0);
            glRotatef((GLfloat)orientation[1], 0, (GLfloat)1, 0);
            glRotatef((GLfloat)orientation[2], 0, 0, (GLfloat)1);
            glTranslatef((GLfloat)position[0], (GLfloat)position[1],
                (GLfloat)position[2]);

            // Draw objects
            float orientationFloor[3] = { 0, 0, 0 }; // Radians
            float positionFloor[3] = { 0, 0, 0 };
            float scaleFloor[3] = { 15, (float)0.05, 13 };
            drawCube(orientationFloor, positionFloor, scaleFloor, (float)0.1);

            drawSphere(10, 10, 3, 0, 0);

            float orientationCube[3] = { 0, rotateCube, 0 }; // Radians
            float positionCube[3] = { 0, 1, 0 };
            float scaleCube[3] = { 1, 1, 1 };
            drawCube(orientationCube, positionCube, scaleCube, (float)0.2);
            rotateCube += (float)0.2;
        }

        // Rendering.
        ImGui::Render();

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
            clear_color.z * clear_color.w, clear_color.w);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
