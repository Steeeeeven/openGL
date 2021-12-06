#include "glad/glad.h" 
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
    glfwInit();
    //配置GLFW选项的值
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //如果使用的是Mac OS X系统  添加这行
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

    //创建一个窗口对象
    GLFWwindow* window = glfwCreateWindow(1200, 900, "StevenTesting", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } 

    //通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    //每次窗口大小被调整的时候被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数   GLFW提供的procAddress
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置渲染窗口（即视口(Viewport)）的尺寸大小
    //glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度
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

    //三个顶点的xyz 
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    }; 
    
    //顶点缓冲对象(VBO)  用于一次性尽可能多的把顶点信息发到显存里
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
    //使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上   只要是不同缓冲类型就可以绑定多个缓冲类型
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //把之前定义的顶点数据复制到缓冲的内存中
    //第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
    //  GL_STATIC_DRAW ：数据不会或几乎不会改变。
    //  GL_DYNAMIC_DRAW：数据会被改变很多。
    //  GL_STREAM_DRAW ：数据每次绘制时都会改变。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //创建一个顶点和片段着色器来处理数据    
    //glCreateShader创建顶点着色器(VERTEX_SHADER)
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //着色器语言GLSL 版本和opengl一致 
    /* 取一个vec3的向量(3d) 但是顶点着色器的输出是vec4的  于是乎手动赋值 */
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    //将着色器代码绑到着色器对象上 编译
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

    //片段着色器(Fragment Shader) 计算像素最后的颜色输出
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


    //着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    //把上述的两个着色器附加到着色器程序上,  然后使用Link连接
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "着色器程序初始化失败\n" << infoLog << std::endl;
        return -1;
    }

    //激活程序对象
    glUseProgram(shaderProgram);

    //此时可以删除着色器对象了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //链接顶点属性, 解释顶点数据
    //TODO要配置属性的顶点、顶点属性的维数、属性数据类型(vec*由浮点数组成)、是否希望数据被标准化(0~1)、
    //步长(顶点属性之间的间隔, 这里是三个float, 并且互相紧密相连)、offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //以顶点属性位置值作为参数，启用顶点属性
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);

    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    

#if 1
    //RenderLoop
    while (!glfwWindowShouldClose(window))
    {
        /* 可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT */
        /* 现在只清除颜色缓存 */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //渲染循环的每一个迭代中调用  这个用来检查esc是否被按下(输入控制)
        std::cout << "loop begin:frame" << frame << std::endl;
        processInput(window);

  
        //交换颜色缓冲   是一个储存着GLFW窗口每一个像素颜色值的大缓冲
        glfwSwapBuffers(window);
        
        // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
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
        //把WindowShouldClose属性设置为 true的方法关闭GLFW
        glfwSetWindowShouldClose(window, true);
    }
}
