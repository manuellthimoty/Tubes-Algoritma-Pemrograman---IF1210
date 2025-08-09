<!-- THIS IS A TEMPLATE DIRECTORY -->
<!-- DELETE THIS README.md IF IT IS NO LONGER NEEDED -->

Use this directory to store your header implementation files. 

gcc main.c c/F01.c c/F02.c c/F03.c c/F04.c c/F05.c c/F06.c c/F10.c -Iheader -o program
gcc main.c c/F01.c c/F02.c c/F03.c c/F04.c c/F05.c c/F06.c -Iheader -o program
./program


gcc main.c c/F01.c c/F02.c c/F03.c c/F04.c c/F05.c c/F06.c c/F10.c c/F11.c c/F14.c c/F15.c c/F18.c -Iheader -o program

gcc main.c c/F01.c c/F02.c c/F03.c c/F04.c c/F05.c c/F06.c c/F07.c c/F08.c c/F09.c c/F10.c c/F11.c c/F12.c c/F13.c c/F14.c c/F15.c c/F16.c c/F17.c c/F18.c c/BackToMenu.c -Iheader -o program

sudo apt install valgrind
valgrind ./program
valgrind --leak-check=full ./program