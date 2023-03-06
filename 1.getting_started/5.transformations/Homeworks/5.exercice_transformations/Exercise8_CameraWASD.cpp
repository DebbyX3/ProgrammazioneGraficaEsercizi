/*
Esercizio: navigare coi tasti WASD nella scena 'spostando' la camera
*/


#include <glad/glad.h>
#include <GLFW/glfw3.h>
//shader
#include <learnopengl/shader_s.h>

#include <stb_image.h>

//glm -> math functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera (posizione iniziale della camera)
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	//tempo fra il frame corrente e l'ultimo frame
float lastFrame = 0.0f;


int main() {
    //Procedura per instanziare la GLFW window (creare la finestra)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //Creiamo la finestra
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//creaiamo il contesto
    //Chiamata alla funzione per la gestione della dimensione della window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Controllo sul caricamento di glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //Abilito il deph test (z-buffer per capire la profondità)
    //Se non abilito la profondità e la cancellazione del buffer con GL_DEPTH_BUFFER_BIT nel render loop non ho profondità
    //Viene renderizzato quindi come elemento in primo piano quello che c'è all'interno del buffer in quel momento
    glEnable(GL_DEPTH_TEST);

    //Leggiamo da file vertex shader e fragment shader
    Shader ourShader("transformation_es7.vs", "transformation_es7.fs");

    //Trattamento dei dati -> cubo: 36 vertici e 6 facce
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    //Posizione dei cubi nel mondo 
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    //Faccio i bind per associare la memoria
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //a VBO associo la struttura dati vertices[]


    //Spiego come sono fatti idati
    //Attributi per la posizione
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Attributi per le coordinate texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //Caricamento e creazione texture
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); //oggetto texture
    //Parametri per il texture wrapping sugli assi s e t
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT (default): ripete la texture nella zona esterna al bordo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //Parametri per il texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_LINEAR: il colore viene influenzato anche dai colori vicini
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Caricamento immagine, creazione della texture e delle mipmap
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); //flip dell'immagine
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        //generazione della texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //generazione delle mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); //oggetto texture
    //Parametri per il texture wrapping sugli assi s e t
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT (default): ripete la texture nella zona esterna al bordo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //Parametri per il texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_LINEAR: il colore viene influenzato anche dai colori vicini
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Caricamento immagine, creazione della texture e delle mipmap
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        //generazione della texture
        //GL_RGBA poichè l'immagine ha un canale alpha (trasparenza)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //generazione delle mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //shader
    ourShader.use(); //chiamato prima degli uniform
    //setto manualmente gli uniform
    //glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    //glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);
    //setto gli uniform attraverso la classe
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    //creaiamo la projection transform fuori dal render loop -> non è necessario aggiornarla frame per frame
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    //Render loop per la window
    //Verifichiamo che la finestra sia aperta
    while (!glfwWindowShouldClose(window)) {
        //Input per la chiusura
        processInput(window);

        //Colore di sfondo
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT); //Cancelliamo il colore del buffer e lo ridefiniamo con quello inserito in glClearColor 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Pulisco anche il buffer

        //Calcolo frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //Attivo lo shader perchè devo passargli le matrici
        ourShader.use();

        //lookAt
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //posiziono la camera avanti di -3.0

        //Definisco le variabili come uniform
        //Rendiamo visibile la trasformazione allo shader dichiarandola nella memoria principale (programma principale -> main)
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        //ourShader.setMat4("projection", projection); //-> passando direttamente dalla classe shader 


        //render dei cubi
        //Per ogni cubo traslo e assegno una rotazione
        glBindVertexArray(VAO); //lo associo alla struttura di memoria VAO 
        for (unsigned int i = 0; i < 10; i++) {
            //calcolo del model per ogni cubo
            glm::mat4 model = glm::mat4(1.0f); //model transform
            model = glm::translate(model, cubePositions[i]);
            //Ogni cubo ha la propria angolazione
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));


            //Rendiamo visibile la trasformazione allo shader dichiarandola nella memoria principale (programma principale -> main)
            unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            //Disegno 10 volte lo stesso oggetto in posizioni diverse
            glDrawArrays(GL_TRIANGLES, 0, 36); //disegno il cubo
        }


        glfwSwapBuffers(window); //Mando nella window quelo che c'è nel buffer (color buffer)
        glfwPollEvents(); //Verifico che non ci siano altri eventi
    }

    // deallocazione della memoria (opzionale)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    //Indichiamo che abbiamo finito di utilizzare le funzioni di glfw e usciamo dal programma
    glfwTerminate(); //ripristiniamo
    return 0;
}


//Funzione che si verifica se si è premuto ESC per chiudere la finestra
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Valore che fa aumentare o diminuire la velocità con la quale muovo la camera
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

//Definizione della callback che gestisce la dimensione della window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}