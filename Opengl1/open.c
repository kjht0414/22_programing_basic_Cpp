
#include"GL/glew.h"
#include"GLFW/glfw3.h"

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

void render(GLFWwindow* window);

int main() {
    // glfw 초기화
    glfwInit();

    // window 만들기 (width, height, title, monitor, share)
    GLFWwindow* window = glfwCreateWindow(640, 480, "Inyong", 0, 0);
    

    // context는 그리기 위한 내용을 담고 있음
    // window가 두개이면 context가 두개일수도 있음
    // 그러면 어떤 context로 그릴지 정해줘야 함
    glfwMakeContextCurrent(window);

    // 사용자가 window 창을 닫을 때까지
    while (!glfwWindowShouldClose(window)) {
        // window 그려주기
        //render(window);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        // 마우스 움직이는 것. 이런것들 들고와서 필요한 window한테 보내주기
        glfwPollEvents();
    }

    // window가 닫혔을 경우 끝내자
    glfwDestroyWindow(window);
    glfwTerminate();
}

void render(GLFWwindow* window) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // 이것이 우리의 버텍스 버퍼를 가리킵니다.
    GLuint vertexbuffer;
    // 버퍼를 하나 생성합니다. vertexbuffer 에 결과 식별자를 넣습니다
    glGenBuffers(1, &vertexbuffer);
    // 아래의 명령어들은 우리의 "vertexbuffer" 버퍼에 대해서 다룰겁니다
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // 우리의 버텍스들을 OpenGL로 넘겨줍니다
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // 0번째 속성(attribute). 0 이 될 특별한 이유는 없지만, 쉐이더의 레이아웃(layout)와 반드시 맞추어야 합니다.
        3,                  // 크기(size)
        GL_FLOAT,           // 타입(type)
        GL_FALSE,           // 정규화(normalized)?
        0,                  // 다음 요소 까지 간격(stride)
        (void*)0            // 배열 버퍼의 오프셋(offset; 옮기는 값)
    );
    // 삼각형 그리기!
    glDrawArrays(GL_TRIANGLES, 0, 3); // 버텍스 0에서 시작해서; 총 3개의 버텍스로 -> 하나의 삼각형
    glDisableVertexAttribArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}