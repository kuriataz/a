#!/bin/bash

# Funkcja do pobierania SHA sumy pliku
get_sha() {
    sha256sum "$1" | awk '{print $1}'
}

if [ -n "$(ls -A Catch2 2>/dev/null)" ]; then

else
    git submodule update --init Catch2
fi


# Sprawdzenie czy plik install_env istnieje
if [ -e "install_env" ]; then
    # Pobranie SHA sumy pliku install_env
    current_sha=$(get_sha "install_env")

    # Sprawdzenie czy istnieje plik .installed
    if [ -e ".installed" ]; then
        # Odczytanie wszystkich SHA sum z pliku .installed
        saved_sha_list=($(cat ".installed"))

        # Sprawdzenie czy aktualna SHA sumy jest wśród zapisanych
        if [[ " ${saved_sha_list[@]} " =~ " $current_sha " ]]; then
            echo "Aktualna SHA sumy jest już zapisana w .installed. Brak konieczności uruchamiania install_env."
        else
            echo "Aktualna SHA sumy nie jest zapisana w .installed. Uruchamianie install_env..."

            # Uruchamianie install_env
            ./install_env

            # Dodanie aktualnej SHA sumy do listy w pliku .installed
            echo "$current_sha" >> ".installed"
        fi
    else
        echo "Tworzenie pliku .installed..."
        echo "$current_sha" > ".installed"

        echo "Uruchamianie install_env..."
        ./install_env
    fi
else
    echo "Błąd: Brak pliku install_env."
fi
