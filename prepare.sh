#!/bin/bash

# Funkcja do pobierania SHA sumy pliku
get_sha() {
    sha256sum "$1" | awk '{print $1}'
}

# Check if Catch2 directory is not empty
if [ ! "$(ls -A Catch2 2>/dev/null)" ]; then
    git submodule update --init Catch2
fi

# Sprawdzenie czy plik install_env istnieje
if [ -e "install_env.sh" ]; then
    # Pobranie SHA sumy pliku install_env
    current_sha=$(get_sha "install_env.sh")

    # Sprawdzenie czy istnieje plik .installed
    if [ -e ".installed" ]; then
        # Odczytanie wszystkich SHA sum z pliku .installed
        saved_sha_list=($(cat ".installed"))

        # Sprawdzenie czy aktualna SHA sumy jest wśród zapisanych
        if [[ ! " ${saved_sha_list[@]} " =~ " $current_sha " ]]; then
            # Uruchamianie install_env
            # ./install_env.sh

            echo "$current_sha" >> ".installed"
        fi
    else
        echo "$current_sha" > ".installed"

        #./install_env.sh
    fi
else
    echo "Error. File install_env.sh not found."
fi
