# Export Manager

A small C++23 application demonstrating PImpl, CRTP and Type Erasure concepts, that formats data to JSON and XML formats and saves in file.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Usage

Modify the Data in the main.cpp and the file name to be saved to.
Build the executable like mentioned above.
Run the executable:

```sh
./build/export-manager
```

Take a look into the new generated files like `fileName_pretty.json`