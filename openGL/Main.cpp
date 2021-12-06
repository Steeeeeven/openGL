#include "glad/glad.h" 
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
    glfwInit();
    //����GLFWѡ���ֵ
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //���ʹ�õ���Mac OS Xϵͳ  �������
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

    //����һ�����ڶ���
    GLFWwindow* window = glfwCreateWindow(1200, 900, "StevenTesting", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } 

    //֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);

    //ÿ�δ��ڴ�С��������ʱ�򱻵���
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���   GLFW�ṩ��procAddress
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //������Ⱦ���ڣ����ӿ�(Viewport)���ĳߴ��С
    //glViewport����ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶�
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //Render Loop
    int frame = 0;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
#if 0
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT);
#endif

    //���������xyz 
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    }; 
    
    //���㻺�����(VBO)  ����һ���Ծ����ܶ�İѶ�����Ϣ�����Դ���
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //���㻺�����Ļ���������GL_ARRAY_BUFFER
    //ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����   ֻҪ�ǲ�ͬ�������;Ϳ��԰󶨶����������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
    //���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
    //  GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
    //  GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
    //  GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //����һ�������Ƭ����ɫ������������    
    //glCreateShader����������ɫ��(VERTEX_SHADER)
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //��ɫ������GLSL �汾��openglһ�� 
    /* ȡһ��vec3������(3d) ���Ƕ�����ɫ���������vec4��  ���Ǻ��ֶ���ֵ */
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    //����ɫ���������ɫ�������� ����
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    //Ƭ����ɫ��(Fragment Shader) ��������������ɫ���
    //RGBA
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }


    //��ɫ������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    //��������������ɫ�����ӵ���ɫ��������,  Ȼ��ʹ��Link����
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "��ɫ�������ʼ��ʧ��\n" << infoLog << std::endl;
        return -1;
    }

    //����������
    glUseProgram(shaderProgram);

    //��ʱ����ɾ����ɫ��������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //���Ӷ�������, ���Ͷ�������
    //TODOҪ�������ԵĶ��㡢�������Ե�ά����������������(vec*�ɸ��������)���Ƿ�ϣ�����ݱ���׼��(0~1)��
    //����(��������֮��ļ��, ����������float, ���һ����������)��offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //�Զ�������λ��ֵ��Ϊ���������ö�������
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. ��VAO
    glBindVertexArray(VAO);

    // 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. ���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    

#if 1
    //RenderLoop
    while (!glfwWindowShouldClose(window))
    {
        /* ���ܵĻ���λ��GL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT */
        /* ����ֻ�����ɫ���� */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //��Ⱦѭ����ÿһ�������е���  ����������esc�Ƿ񱻰���(�������)
        std::cout << "loop begin:frame" << frame << std::endl;
        processInput(window);

  
        //������ɫ����   ��һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳�
        glfwSwapBuffers(window);
        
        // �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwPollEvents();
        std::cout << "loop end:frame" << frame++ << std::endl;
    }

    glfwTerminate();
    return 0;
#endif
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "window size is being changing" << std::endl;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        //��WindowShouldClose��������Ϊ true�ķ����ر�GLFW
        glfwSetWindowShouldClose(window, true);
    }
}
