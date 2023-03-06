/*
Esercizio: projection transform!
*/

/*
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

    //Leggiamo da file vertex shader e fragment shader
    Shader ourShader("transformation_es4.vs", "transformation_es4.fs");

    //Trattamento dei dati -> in realtà in questo caso i colori non li uso perchè ho la texture
    float vertices[] = {
        //posizioni           //coordinate texture 
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    //I vertici della struttura sono associati agli indici
    unsigned int indices[] = {
        0, 1, 3, // primo triangolo
        1, 2, 3  // secondo triangolo
    };

    unsigned int VBO, VAO, EBO; //genero le strutture di supporto VBO, VAO e EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); //buffer legato agli indici

    //Faccio i bind per associare la memoria
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //a VBO associo la struttura dati vertices[]

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //a EBO associo la struttura indices[]

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



    //Render loop per la window
    //Verifichiamo che la finestra sia aperta
    while (!glfwWindowShouldClose(window))
    {
        //Input per la chiusura
        processInput(window);

        //Colore di sfondo
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); //Cancelliamo il colore del buffer e lo ridefiniamo con quello inserito in glClearColor 

        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //Attivo lo shader perchè devo passargli le matrici
        ourShader.use();

        //Creo la trasformazione
        glm::mat4 model = glm::mat4(1.0f); //model transform
        glm::mat4 view = glm::mat4(1.0f); //view transform
        glm::mat4 projection = glm::mat4(1.0f); //projection transform
        //Creiamo per l'oggetto una rotazione di 55 gradi lungo l'asse x
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); //posiziono la camera avanti di -3.0
                                      //field of view       //dimensioni                         //near plane 
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                                                                                                       //far plane

        //Definisco le variabili come uniform
        //Rendiamo visibile la trasformazione allo shader dichiarandola nella memoria principale (programma principale -> main)
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
        //ourShader.setMat4("projection", projection); -> passando direttamente dalla classe shader 


        //render delle texture
        //ourShader.use(); //shader creato dai 2 file sorgente
        glBindVertexArray(VAO); //lo associo alla struttura di memoria VAO 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //disegno i 2 triangoli che formano la texture rettangolare

        glfwSwapBuffers(window); //Mando nella window quello che c'è nel buffer (color buffer)
        glfwPollEvents(); //Verifico che non ci siano altri eventi
    }

 // deallocazione della memoria (opzionale)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    //Indichiamo che abbiamo finito di utilizzare le funzioni di glfw e usciamo dal programma
    glfwTerminate(); //ripristiniamo
    return 0;
}


//Funzione che si verifica se si è premuto ESC per chiudere la finestra
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//Definizione della callback che gestisce la dimensione della window
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}*/