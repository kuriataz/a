#!/bin/bash

# Sprawdź, czy użytkownik podał ścieżkę docelową
if [ -z "$1" ]; then
    echo "Podaj ścieżkę docelową dla instalacji biblioteki."
    exit 1
fi

# Ustal ścieżkę docelową
install_path=$(realpath "$1")

# Ustal ścieżkę źródłową
source_dir="$2"

# Stwórz katalog budowy
build_dir="build"
mkdir -p "${build_dir}"

# Przejdz do katalogu budowy
cd "${build_dir}" || exit

# Uruchom CMake w celu konfiguracji i generacji plików Makefile
cmake -DCMAKE_CURRENT_SOURCE_DIR="${source_dir}" ..

# Wykonaj instalację za pomocą CMake
cmake --install . --prefix "${install_path}"

# Stwórz katalog include, jeśli nie istnieje
include_dir="${install_path}/include"
mkdir -p "${include_dir}"

# Znajdź pliki nagłówkowe
header_files=$(ls "${source_dir}"/*.hpp)

# Przenieś pliki nagłówkowe do katalogu include
for header_file in ${header_files}; do
    base_name=$(basename "${header_file}")
    cp "${header_file}" "${include_dir}/${base_name}"
done

echo "Biblioteka zainstalowana w: ${install_path}"
