#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

void OcultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = FALSE;
    cci.dwSize = 1;

    SetConsoleCursorInfo(hCon,&cci);
}

int main()
{
    /*
        HANDLE hCon; //hCon es un nombre cualquiera
        hCon = GetStdHandle(STD_OUTPUT_HANDLE); // se recupera el identificador de la consola
        // se toma control de la salida, porque la idea es poner un caracter en pantalla es algo de salida

        COORD dwPos; // posición x y
        dwPos.X = 0;
        dwPos.Y = 0;

        SetConsoleCursorPosition(hCon, dwPos); // de windows.h
        // handle: es como indentificador de una ventana en windows.
        // es decir esta función identifica en cual ventana se va a imprimir el caracter * en este caso
        //printf("*");
    */

    OcultarCursor();

    bool game_over = false;
    int x = 0, y = 0;
    gotoxy(x,y); printf("*");

    while (!game_over){

        if(kbhit()){ // kbhit (de conio.h) detecta si el usario apretó alguna tecla

            char tecla = getch(); // getch obtiene el caracter que se apretó
            gotoxy(x,y); printf(" ");
            if (tecla == 'a' || tecla == IZQUIERDA)
                x--;
            if (tecla == 'd' || tecla == DERECHA)
                x++;
            if (tecla == 'w' || tecla == ARRIBA)
                y--;
            if (tecla == 's' || tecla == ABAJO)
                y++;

            gotoxy(x,y); printf("*");
        }

        Sleep(30);// 30 milisegundos
    }

    return 0;
}
