#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>

#include <iostream>
#include <vector>

// defining functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::vector<float> genArray(int numOfPoints);
glm::vec3 genNormals(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // D:\OpenglProject\shaders\shader.frag
    // C:\Projects\Graphics Engine\shaders\lightingShader.frag
    Shader objectShader("C:/Projects/GraphicsEngine/shaders/lightingShader.vert", "C:/Projects/GraphicsEngine/shaders/lightingShader.frag");
    Shader cubeShader("C:/Projects/GraphicsEngine/shaders/lightSource.vert", "C:/Projects/GraphicsEngine/shaders/lightSource.frag");

    
    int numOfPoints = 12;
    std::vector<float> vectorVertex = genArray(numOfPoints);
    float vertex[vectorVertex.size()];

    for (int i = 0; i < vectorVertex.size(); i++) {
        vertex[i] = vectorVertex[i];
    }

    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
    
    // configure torus's vao and vbo
    unsigned int torusVBO, torusVAO;
    glGenVertexArrays(1, &torusVAO);
    glGenBuffers(1, &torusVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, torusVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // position attribute
    glBindVertexArray(torusVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // surface normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // configure cube vao and vbo
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // uncomment for wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        objectShader.use();
        objectShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        objectShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        objectShader.setVec3("lightPos", lightPos);

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        objectShader.setMat4("model", model);
        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);

        // render torus
        glBindVertexArray(torusVAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex)/sizeof(float));

        // render light source
        cubeShader.use();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        cubeShader.setMat4("model", model);
        
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &torusVAO);
    glDeleteBuffers(1, &torusVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    
    glfwTerminate();
    return 0;
}

std::vector<float> genArray(int numOfPoints) {
    std::vector<float> vertex;

    float twoPI = 6.28318530718f;
    float theta_spacing = twoPI / numOfPoints;
    float phi_spacing = twoPI / numOfPoints;
    int R1 = 1;
    int R2 = 2;

    // calculates all the vertices on the torus
    for (float theta = theta_spacing; theta < twoPI; theta += theta_spacing) {
        float costheta = cos(theta), sintheta = sin(theta);

        for (float phi = phi_spacing; phi < twoPI; phi += phi_spacing) {
            float cosphi = cos(phi), sinphi = sin(phi);

            // x, y coordinate of the circle, before revolving
            float circlex = R2 + R1*costheta;
            float circley = R1*sintheta;
            float x = circlex*cosphi;
            float y = circley;
            float z = -1*circlex*sinphi;
            vertex.push_back(x);
            vertex.push_back(y);
            vertex.push_back(z);
        }
    }
    
    // formats the vertices
    std::vector<float> newVertex;
    for (int i = 0; i < numOfPoints; i++) {
        for (int j = 0; j < numOfPoints; j++) {
            newVertex.push_back(vertex[i*3 + j*numOfPoints*3]);
            newVertex.push_back(vertex[i*3 + j*numOfPoints*3 + 1]);
            newVertex.push_back(vertex[i*3 + j*numOfPoints*3 + 2]);
        }
    }

    // adds all triangle vertices
    std::vector<float> nVertex;
    for (int i = 0; i < numOfPoints; i++) {
        for(int j = 0; j < numOfPoints; j++) {
            int offset = i*numOfPoints*3;

            // first triangle
            // adds first vertex (x,y,z)
            nVertex.push_back(newVertex[offset + j*3]);
            nVertex.push_back(newVertex[offset + j*3 + 1]);
            nVertex.push_back(newVertex[offset + j*3 + 2]);

            // adds second vertex (x,y,z)
            if (i+1 != numOfPoints) {
                nVertex.push_back(newVertex[offset + (j+numOfPoints)*3]);
                nVertex.push_back(newVertex[offset + (j+numOfPoints)*3 + 1]);
                nVertex.push_back(newVertex[offset + (j+numOfPoints)*3 + 2]);
            } else {
                nVertex.push_back(newVertex[j*3]);
                nVertex.push_back(newVertex[j*3 + 1]);
                nVertex.push_back(newVertex[j*3 + 2]);
            }

            // adds third vertex (x,y,z) and checks if the third vertex needs to connect to the first point
            if (j+1 != numOfPoints) {
                nVertex.push_back(newVertex[offset + (j+1)*3]);
                nVertex.push_back(newVertex[offset + (j+1)*3 + 1]);
                nVertex.push_back(newVertex[offset + (j+1)*3 + 2]);
            } else {
                nVertex.push_back(newVertex[offset]);
                nVertex.push_back(newVertex[offset + 1]);
                nVertex.push_back(newVertex[offset + 2]);
            }

            // second triangle
            // add to flag when the point is out of bounds of the array
            int flag = 0;

            // first point
            if (i+1 != numOfPoints) {
                nVertex.push_back(newVertex[offset + (j+numOfPoints)*3]);
                nVertex.push_back(newVertex[offset + (j+numOfPoints)*3 + 1]);
                nVertex.push_back(newVertex[offset + (j+numOfPoints)*3 + 2]);
            } else {
                nVertex.push_back(newVertex[j*3]);
                nVertex.push_back(newVertex[j*3 + 1]);
                nVertex.push_back(newVertex[j*3 + 2]);
                flag++;
            }
            
            // second point
            if (j+1 != numOfPoints) {
                nVertex.push_back(newVertex[offset + (j+1)*3]);
                nVertex.push_back(newVertex[offset + (j+1)*3 + 1]);
                nVertex.push_back(newVertex[offset + (j+1)*3 + 2]);
            } else {
                nVertex.push_back(newVertex[offset]);
                nVertex.push_back(newVertex[offset + 1]);
                nVertex.push_back(newVertex[offset + 2]);
                flag+=2;
            }

            // third point
            switch (flag) {
                // out of bounds in the x direction
                case 1:
                    nVertex.push_back(newVertex[(j+1)*3]);
                    nVertex.push_back(newVertex[(j+1)*3 + 1]);
                    nVertex.push_back(newVertex[(j+1)*3 + 2]);
                    break;
                // out of bounds in the y direction
                case 2:
                    nVertex.push_back(newVertex[offset + numOfPoints*3]);
                    nVertex.push_back(newVertex[offset + numOfPoints*3 + 1]);
                    nVertex.push_back(newVertex[offset + numOfPoints*3 + 2]);
                    break;
                // out of bounds in both directions
                case 3:
                    nVertex.push_back(newVertex[0]);
                    nVertex.push_back(newVertex[1]);
                    nVertex.push_back(newVertex[2]);
                    break;
                default:
                    nVertex.push_back(newVertex[offset + (j+numOfPoints+1)*3]);
                    nVertex.push_back(newVertex[offset + (j+numOfPoints+1)*3 + 1]);
                    nVertex.push_back(newVertex[offset + (j+numOfPoints+1)*3 + 2]);
            }

        }
    }

    // calculates surface normals of each triangle and adds it to the vertex array
    bool topTriangle = true;
    for (int i = 0; i < nVertex.size(); i+=18) {
        // grabs the three verticies of the triangle
        // turns each vertex into a vector of coordinates x, y, z
        glm::vec3 p1, p2, p3;

        if (topTriangle) {
            p1 = glm::vec3(nVertex[i], nVertex[i+1], nVertex[i+2]);
            p2 = glm::vec3(nVertex[i+3], nVertex[i+4], nVertex[i+5]);
            p3 = glm::vec3(nVertex[i+6], nVertex[i+7], nVertex[i+8]);
        } else {
            p1 = glm::vec3(nVertex[i], nVertex[i+1], nVertex[i+2]);
            p2 = glm::vec3(nVertex[i+6], nVertex[i+7], nVertex[i+8]);
            p3 = glm::vec3(nVertex[i+3], nVertex[i+4], nVertex[i+5]);
        }
        
        glm::vec3 Normal = genNormals(p1, p2, p3);

        for (int j = 0; j < 3; j++) {
            // inserts normals into the existing vertex array
            nVertex.insert(nVertex.begin() + i+3 + j*6, Normal.x);
            nVertex.insert(nVertex.begin() + i+4 + j*6, Normal.y);
            nVertex.insert(nVertex.begin() + i+5 + j*6, Normal.z);
        }
        topTriangle = !topTriangle;
    }

    return nVertex;
}

glm::vec3 genNormals(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    // U and V are the two edges of the triangle
    glm::vec3 U = p2 - p1;
    glm::vec3 V = p3 - p1;
    
    // N is the normal
    glm::vec3 N;
    N.x = U.y*V.z - U.z*V.y;
    N.y = U.z*V.x - U.x*V.z;
    N.z = U.x*V.y - U.y*V.x;

    return N;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}