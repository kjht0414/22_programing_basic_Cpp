
#include"GL/glew.h"
#include"GLFW/glfw3.h"

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

void render(GLFWwindow* window);

int main() {
    // glfw �ʱ�ȭ
    glfwInit();

    // window ����� (width, height, title, monitor, share)
    GLFWwindow* window = glfwCreateWindow(640, 480, "Inyong", 0, 0);
    

    // context�� �׸��� ���� ������ ��� ����
    // window�� �ΰ��̸� context�� �ΰ��ϼ��� ����
    // �׷��� � context�� �׸��� ������� ��
    glfwMakeContextCurrent(window);

    // ����ڰ� window â�� ���� ������
    while (!glfwWindowShouldClose(window)) {
        // window �׷��ֱ�
        //render(window);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        // ���콺 �����̴� ��. �̷��͵� ���ͼ� �ʿ��� window���� �����ֱ�
        glfwPollEvents();
    }

    // window�� ������ ��� ������
    glfwDestroyWindow(window);
    glfwTerminate();
}

void render(GLFWwindow* window) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // �̰��� �츮�� ���ؽ� ���۸� ����ŵ�ϴ�.
    GLuint vertexbuffer;
    // ���۸� �ϳ� �����մϴ�. vertexbuffer �� ��� �ĺ��ڸ� �ֽ��ϴ�
    glGenBuffers(1, &vertexbuffer);
    // �Ʒ��� ��ɾ���� �츮�� "vertexbuffer" ���ۿ� ���ؼ� �ٷ�̴ϴ�
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // �츮�� ���ؽ����� OpenGL�� �Ѱ��ݴϴ�
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // 0��° �Ӽ�(attribute). 0 �� �� Ư���� ������ ������, ���̴��� ���̾ƿ�(layout)�� �ݵ�� ���߾�� �մϴ�.
        3,                  // ũ��(size)
        GL_FLOAT,           // Ÿ��(type)
        GL_FALSE,           // ����ȭ(normalized)?
        0,                  // ���� ��� ���� ����(stride)
        (void*)0            // �迭 ������ ������(offset; �ű�� ��)
    );
    // �ﰢ�� �׸���!
    glDrawArrays(GL_TRIANGLES, 0, 3); // ���ؽ� 0���� �����ؼ�; �� 3���� ���ؽ��� -> �ϳ��� �ﰢ��
    glDisableVertexAttribArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}