# Встановлюємо мінімально необхідну версію CMake
cmake_minimum_required(VERSION 3.20)

# Оголошуємо проєкт та мову програмування
project(kursach LANGUAGES CXX)

# Встановлюємо стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматично знаходимо абсолютно всі файли .cpp у папці src
file(GLOB APP_SOURCES "src/*.cpp")

# Створюємо виконуваний файл з усіх знайдених .cpp файлів
add_executable(kursach ${APP_SOURCES})

# Вказуємо компілятору, де шукати заголовочні файли (.h)
target_include_directories(kursach PRIVATE include)

# --- ВИПРАВЛЕНИЙ БЛОК ДЛЯ КОРЕКТНОЇ РОБОТИ З КИРИЛИЦЕЮ ---
# Тепер він перевіряє тип компілятора, а не тільки ОС
if(MSVC)
    # Прапор для компілятора Microsoft Visual C++
    target_compile_options(kursach PRIVATE /utf-8)
else()
    # Прапори для компіляторів GCC та Clang (включно з MinGW)
    target_compile_options(kursach PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()