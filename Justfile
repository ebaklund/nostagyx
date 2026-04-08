init:
    #!/usr/bin/env bash
    # https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux
    # https://github.com/raylib-extras/raylib-quickstart
    sudo apt install build-essential git
    sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
    sudo apt-get install libzip-dev

install-raylib-cpp:
    #!/usr/bin/env bash

    TARGET_DIR="/usr/include"
    TMP_DIR="/tmp/raylib-cpp"

    if [[ -f "$TARGET_DIR/raylib-cpp.hpp" ]]; then
        echo "raylib-cpp is already installed."
    else
        mkdir -p $TMP_DIR
        cd $TMP_DIR
        wget https://github.com/RobLoach/raylib-cpp/archive/refs/tags/v5.5.1.tar.gz -O raylib-cpp-v5.5.1.tar.gz
        gunzip raylib-cpp-v5.5.1.tar.gz
        tar -xvf raylib-cpp-v5.5.1.tar
        sudo cp ./raylib-cpp-5.5.1/include/*.hpp $TARGET_DIR/
        echo "Done."
    fi

build:
    #!/usr/bin/env bash
    cd src
    make build

run:
    #!/usr/bin/env bash
    cd src
    make clean
    make run
