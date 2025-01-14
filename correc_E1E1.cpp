#include <iostream>
#include <GLFW/glfw3.h>
#include <omp.h>
#include <math.h>

#define LARGO 400
#define ALTO 200
const int TAMANIO_PALETA_COLORES = 16;
static GLFWwindow *ventana = nullptr;
static GLuint *buffer_pixeles = nullptr;
GLuint id_textura;

uint32_t de_RGBA_a_ARGB(uint32_t a) {
    return
            ((a & 0X000000FF) << 24) |
            ((a & 0X0000FF00) << 8) |
            ((a & 0x00FF0000) >> 8) |
            ((a & 0xFF000000) >> 24);
}

const GLuint paleta_colores[] = {
    de_RGBA_a_ARGB(0xFF1010FF),
    de_RGBA_a_ARGB(0xEF1019FF),
    de_RGBA_a_ARGB(0xE01123FF),
    de_RGBA_a_ARGB(0xD1112DFF),
    de_RGBA_a_ARGB(0xC11237FF),
    de_RGBA_a_ARGB(0xB21341FF),
    de_RGBA_a_ARGB(0xA3134BFF),
    de_RGBA_a_ARGB(0x931455FF),
    de_RGBA_a_ARGB(0x84145EFF),
    de_RGBA_a_ARGB(0x751568FF),
    de_RGBA_a_ARGB(0x651672FF),
    de_RGBA_a_ARGB(0x56167CFF),
    de_RGBA_a_ARGB(0x471786FF),
    de_RGBA_a_ARGB(0x371790FF),
    de_RGBA_a_ARGB(0x28189AFF),
    de_RGBA_a_ARGB(0x1919A4FF)
};

void dibujar_sin_omp() {
    for (int idx = 0; idx < LARGO * ALTO; idx++) {
        int i = idx % LARGO;
        int j = idx / LARGO;
        int index = i * TAMANIO_PALETA_COLORES / LARGO;
        buffer_pixeles[j * LARGO + i] = paleta_colores[TAMANIO_PALETA_COLORES - index - 1];
    }
}

void dibujar_openmp_loop_work_sharing_21() {
#pragma omp parallel
    {
#pragma omp for
        for (int idx = 0; idx < LARGO * ALTO; idx++) {
            int i = idx % LARGO;
            int j = idx / LARGO;
            int index = i * TAMANIO_PALETA_COLORES / LARGO;
            buffer_pixeles[j * LARGO + i] = paleta_colores[TAMANIO_PALETA_COLORES - index - 1];
        }
    }
}

void dibujar_openmp_parallel_regions_22() {
    int tamanio = LARGO * ALTO;
#pragma omp parallel
    {
        int id_hilo = omp_get_thread_num();
        int num_hilos = omp_get_num_threads();

        int bloque_por_hilo = ceil(tamanio / num_hilos); // Redondea hacia arriba
        int inicio = id_hilo * bloque_por_hilo;
        int fin = (id_hilo + 1) * bloque_por_hilo;

        if (fin > tamanio) {
            fin = tamanio;
        }

        for (int idx = inicio; idx < fin; idx++)
        {
            int i = idx % LARGO;
            int j = idx / LARGO;

            int index = i * TAMANIO_PALETA_COLORES / LARGO;

            buffer_pixeles[j * LARGO + i] = paleta_colores[TAMANIO_PALETA_COLORES - index - 1];
        }
    }
}

void dibujar_openmp_parallel_regions_and_work_sharing_23() {
#pragma omp parallel for
        for (int idx = 0; idx < LARGO * ALTO; idx++) {
            int i = idx % LARGO;
            int j = idx / LARGO;
            int index = i * TAMANIO_PALETA_COLORES / LARGO;
            buffer_pixeles[j * LARGO + i] = paleta_colores[TAMANIO_PALETA_COLORES - index - 1];
    }
}

void pintar() {
    //dibujar_sin_omp();
    //dibujar_openmp_loop_work_sharing_21();
    //dibujar_openmp_parallel_regions_22();
    dibujar_openmp_parallel_regions_and_work_sharing_23();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id_textura);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 LARGO, ALTO, 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 buffer_pixeles);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2f(-1, -1);
    glTexCoord2f(0, 0); glVertex2f(-1, 1);
    glTexCoord2f(1, 0); glVertex2f(1, 1);
    glTexCoord2f(1, 1); glVertex2f(1, -1);
    glEnd();
}

void bucle() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Establece el color de fondo de la ventana.

    while (!glfwWindowShouldClose(ventana)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pintar();

        glFlush();
        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }
}

void inicializar_texturas() {
    glGenTextures(1, &id_textura); // Genera un id para la textura.
    glBindTexture(GL_TEXTURE_2D, id_textura); // Asocia la textura con el id generado.
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA8,
                 LARGO, ALTO, 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 NULL
    ); // Define las características de la textura, como el tamaño y el formato.
    // Configura los parámetros de la textura para que se vea bien al agrandar o achicar.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0); // Desasocia la textura con el id generado.
}

void inicio() {
    // ********************************  1. Inicializar GLFW **********************************
    if (!glfwInit()) {
        exit(-1);
    }

    // OPCIONAL: Configurar GLFW
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);      // La ventana es visible
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);    // La ventana es redimensionable
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Doble buffer para evitar el parpadeo de la pantalla al dibujar_sin_omp

    // ********************************** 2. Crear la ventana **********************************
    ventana = glfwCreateWindow(LARGO, ALTO, "Mi ventana", NULL, NULL);
    if (!ventana) {
        glfwTerminate();
        exit(-1);
    }

    // OPCIONALES:
    // Configurar los callbacks de la ventana.
    // Callback para cerrar la ventana al presionar la tecla ESC
    glfwSetKeyCallback(ventana, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    // Callback para redimensionar la ventana
    glfwSetFramebufferSizeCallback(ventana, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // ****************  3. Hacer que el contexto de OpenGL de la ventana actual *******************
    glfwMakeContextCurrent(ventana);

    // ********************************** 4. Inicializar OpenGL *************************************
    glViewport(0, 0, LARGO, ALTO); // 0,0 es la esquina inferior izquierda y LARGO, ALTO es la esquina superior derecha. Establece el tamaño de la ventana.
    glMatrixMode(GL_PROJECTION); // Configura la matriz de proyección, que determina cómo se proyecta el mundo 3D en la pantalla 2D.
    glLoadIdentity(); // Carga la matriz identidad en la matriz de proyección.
    glOrtho(-1, 1, -1, 1, -1, 1); // Define la proyección ortogonal.
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D); // Habilita el uso de texturas

    // *********************************** 5. Configurar el v-sync ***********************************
    glfwSwapInterval(1); // v-sync activado para evitar el desgarro de la pantalla al dibujar a una velocidad superior a la de refresco de la pantalla.

    // ********************************* 6. Inicializar las texturas *********************************
    inicializar_texturas();

    // **************************** 7. Renderizar la textura con el bucle ******************************
}

void correr() {
    inicio();
    bucle();
    glfwTerminate();
}

int main() {
    buffer_pixeles = new GLuint[LARGO * ALTO];
    correr();
    delete[] buffer_pixeles;
    return 0;
}
