## Come avviare un nuovo progetto OpenGL

(in questa repo  ci sono le directory libs e include + glad e stbimage ma solo just for reference, i progetti VStudio le prendono dai path sotto!)

 - metti progetto a 64 bit in alto da tendina
 - Tasto destro sul nome del progetto nell'esplora soluzioni ->
   proprietà  Proprietà di configurazione -> directory di VCC+
   
 - aggiungere a directory di inclusione il percorso: 
    `C:\Program
   Files\OpenGL\include; `
 - aggiungere a directory librerie il percorso:
   `C:\Program Files\OpenGL\libs;`
   
 - poi in Linker -> input mettere in dipendenze aggiuntive:
   `glfw3.lib;opengl32.lib;` (bastano solo i nomi)
   
 - includi nel progetto file (i primi due sono in `C:\ProgramFiles\OpenGL`):
    -  `glad.c`
    - `stb_image.cpp`
    - `C:\ProgramFiles\OpenGL\libs\assimp[...].lib`
    
-  cambia il path di immagini e shaders, vedi sotto
   
   (Questo non vale più, **vedi sotto** per i path relativi: 
   prima usavo la sintassi `..//..//file.path` tipo: 
    - `Shader ourShader("..//..//3.3.shader.vs", "..//..//3.3.shader.fs");` 
   
     -  nei progetti ho normalizzato tutto mettendo i file
   direttamente nella root, quindi basta scrivere:
   `Shader ourShader("3.3.shader.vs", "3.3.shader.fs");`
   
-  togli poi l'include 
   `#include <learnopengl/filesystem.h>`
   
- modifica le righe tipo 
   - `unsigned int normalMap  = loadTexture(FileSystem::getPath("resources/textures/brickwall_normal.jpg").c_str());`
in 
   - `unsigned int normalMap = loadTexture("brickwall_normal.jpg");`
    
   - metti quella texture nella root del progetto

