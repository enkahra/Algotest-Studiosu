# CMake versiyonunu belirt
cmake_minimum_required(VERSION 3.10)

# Proje adını ve C++ standardını belirle
project(AlgotestStudiosu VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Çalıştırılabilir dosyayı oluştur
# "AlgotestApp" -> oluşacak programın adı
# "src/main.cpp" -> kaynak kod dosyası
add_executable(AlgotestApp 
    core/src/main.cpp
    core/src/DataHandler.cpp
    core/src/SimpleStrategy.cpp
    core/src/Portfolio.cpp
    core/src/Backtester.cpp
)
target_include_directories(AlgotestApp PUBLIC core/include)