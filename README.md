# DLLReplacer

Easy live DLL reloading for faster development iteration with minimal integration effort


# Dependencies
[Detours](https://github.com/microsoft/Detours)


# How to use the example

## Initial setup:
1. Clone/download the repository
2. Build the projects
3. Copy the following files: 
    rename_old.bat, rename_old.py, delete_old.bat, replace_dll.bat, dll_replacer_config.txt) 
  to the compiled executables diretory 
    ./x64/debug/

## Live Reloading:
1. Run Example.exe
2. Modify ExampleFunc() inside ExampleDLL
3. Run rename_old.bat
4. Compile the modified Example.DLL
5. Run replace_dll.bat and watch how it calls to modified function
6. Repeat steps 2-5 as many times as you want


For more information check this post:
https://a10nw01f.github.io/live_dll_reloading/
