# Help from Gemini
# La version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Le nom de ton bébé
project(FourierEpicycles)

# 1. On liste les fichiers C++ à compiler pour créer l'exécutable
# Si tu rajoutes des fichiers .cpp plus tard, il faudra les lister ici !
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Epicycle.cpp
    src/MathParser.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imgui_demo.cpp 
    vendor/rlImGui/rlImGui.cpp
)

# 2. On indique où trouver les fichiers d'en-tête (tes .hpp et le raylib.h)
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    vendor/raylib/include
    vendor/imgui
    vendor/rlImGui
)

target_compile_options(${PROJECT_NAME} PRIVATE /bigobj)

# 3. On indique où trouver le code pré-compilé (le dossier qui contient les .lib ou .a)
target_link_directories(${PROJECT_NAME} PRIVATE 
    vendor/raylib/lib
)

# 4. L'édition de liens (on scotch tous les morceaux ensemble)
# Note : Sur Windows, Raylib a besoin de ces 3 librairies système pour interagir avec la carte graphique et la fenêtre
target_link_libraries(${PROJECT_NAME} PRIVATE 
    raylib 
    opengl32 
    gdi32 
    winmm
)