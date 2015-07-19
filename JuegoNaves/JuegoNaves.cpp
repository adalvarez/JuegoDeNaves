/* Un agradecimiento a Deivid Mendoza y a Código Facilito
 * Escrito y mejorado por Adrián Álvarez @chapitro
 */

#include <stdio.h> // printf
#include <windows.h> // set console
#include <conio.h> // escucha el teclado y responde a el, y getch();
#include <stdlib.h> // random
#include <list> // lista de objetos
using namespace std;

#define ARRIBA 72 // valor correspondiente con el caracter asociado a la tecla arriba
#define IZQUIERDA 75 // valor correspondiente con el caracter asociado a la tecla izquierda
#define DERECHA 77 // valor correspondiente con el caracter asociado a la tecla derecha
#define ABAJO 80 // valor correspondiente con el caracter asociado a la tecla abajo
#define ESPACIO 32 // valor correspondiente con el caracter asociado a la tecla space

/*
 * Esta controla un sleep que tiene dos objetivos:
 *   - el juego se basa en una iteración while por lo tanto la idea aliviar un poco las cantidad de iteraciones.
 *   - esto también influye en la rapidez de los asteroides es por ello que cuando se destruyen los asteroides
 *     este ime va disminuyendo.
 * Aunque este valor principalmente se actualiza cuando hay una colisión bala-asteroide la idea que esté fuera de todo
 *  es para que sea totalmente global para diferentes métodos de algunos objetos, por ejemplo cuando el objeto de tipo nave
 *  se destruye en su método morir(); este time se reinicia para que empiece nuevamente en un nivel bajo.
 */
int time = 50;


void gotoxy(int x, int y){
    /*
     * Función: Dado un x y se prepara las funciones necesarias para
     * poder poner en pantalla un caracter o un conjunto de caracteres.
    */
    HANDLE hCon; // objeto de tipo HANDLE
    hCon = GetStdHandle(STD_OUTPUT_HANDLE); // id ventana
    COORD dwPos; // objeto de tipo COORD
    dwPos.X = x; // pos x
    dwPos.Y = y; //pos y
    SetConsoleCursorPosition(hCon, dwPos);
}

void OcultarCursor(){
    /*
     * Función utilizada para ocultar el cursor en este caso de tipo caracter.
    */
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = FALSE; // se oculta el cursor
    cci.dwSize = 1;

    SetConsoleCursorInfo(hCon,&cci);
}

void pintar_limites(){

    for (int i = 2; i < 78; i++){ // arriba y abajo
        gotoxy(i,3); printf("%c",205);
        gotoxy(i,33); printf("%c",205);
    }

    for (int i = 4; i < 33; i++){ // laterales
        gotoxy(2,i); printf("%c",186);
        gotoxy(77,i); printf("%c",186);
    }
    // cuatro esquinas del marco
    gotoxy(2,3); printf("%c",201);
    gotoxy(2,33); printf("%c",200);
    gotoxy(77,3); printf("%c",187);
    gotoxy(77,33); printf("%c",188);
}

class NAVE{
    //variables privadas
    int x,y; // coordenadas
    int corazones;
    int vidas;
public:

    NAVE( int _x, int _y, int _corazones, int _vidas); // constructor
    void pintar (); // pintar la nave
    void borrar (); // borrar rastro
    void mover (); // movilidad de la nave
    void pintar_corazones();
    void morir(); // cuando se acaban los corazones se pierde una vida
    /*
     * Para la interacción entre clases no se tiene la facilidad de poder acceder
     * a las coordenadas de un objeto de tipo nave, asteroide o bala
     * es por ello que los siguiente métodos retorna "el valor" de las coordenadas
     * más no ocurrirá niguna modificación de la misma de un objeto impropio.
     */
    int X (){return x;}
    int Y (){return y;}
    int VIDAS (){ return vidas;}
    void COR(){corazones--;}

};

NAVE::NAVE(int _x, int _y, int _corazones, int _vidas){ // Método constructor
    x = _x;
    y = _y;
    corazones = _corazones;
    vidas = _vidas;
}
// alternativa de hacer el constructor:
// NAVE::NAVE(int _x, int _y): x(_x), y(_y){}

void NAVE::pintar(){ // Método de clase
    /*
     * USO: para evitar que exista un rastro se debe borrar todo y volverlo a pintar'.
     *      cuando se pierden los 3 corazones se debe volver a pintar' la nave.
     * Pinta el dibujo de la nave
     * El %c corresponde a un caracter ascii por eso después se pone el número.
     */
    gotoxy(x,y);printf("  %c  ",30);
    gotoxy(x,y+1); printf(" %c%c%c ",40,207,41);
    gotoxy(x,y+2); printf("%c%c %c%c",30,190,190,30);
}

void NAVE::borrar(){ // Método de clase
    /*
     * USO: para evitar que exista un rastro se debe borrar' todo y volverlo a pintar.
     *      cuando se pierden los 3 corazones se debe borrar' y volver a pintar la nave.
     * Para borrrar el rasto básicamente se pone todo el espacio que contenía la nave
     *  con espacios en blanco.
     */
    gotoxy(x,y); printf("        ");
    gotoxy(x,y+1); printf("        ");
    gotoxy(x,y+2); printf("        ");
}

void NAVE::mover(){ // Método de clase

    if(kbhit()){ // kbhit() valor booleano que indica que se apretó una tecla

        char tecla = getch(); // obtiene cual tecla se ha apretado
        borrar(); // borra el rastro anterior
        /*
         * Se contempla el uso de las teclas aswd o flechas, pero además
         * se considera que no se pase de los bordes del marco para que
         * se mantenga el orden y no tener efectos extraños
         * además de que se no se borre el marco.
         */
        if ((tecla == 'a' || tecla == IZQUIERDA) && x>3 )
            x-=2; // movilidad
        if ((tecla == 'd' || tecla == DERECHA) && x+6<75)
            x+=2; // movilidad
        if ((tecla == 'w' || tecla == ARRIBA) && y>4)
            y-=2; // movilidad
        if ((tecla == 's' || tecla == ABAJO) && y+3 < 33)
            y+=2; // movilidad
        if (tecla == 'e') corazones--;
        pintar(); // se pinta nuevamente pero con las nuevas coordenadas X y Y
        pintar_corazones(); // se pinta la información de vida y corazones
    }
}

void NAVE::pintar_corazones(){ // Método de clase
    // imprime la cantidad de vidas
    gotoxy(50,2); printf("Chance %d", vidas);
    /*
     * Primero se borra los corazones y se procede a volverlos a pintar
     *  según la cantidad que sea (probablemente se ha disminuido)
     */
    gotoxy(64,2); printf("Life:");
    gotoxy(70,2); printf("      ");
    for (int i = 0; i < corazones; i++){
        // se imprimen cantidad que indique el entero 'corazones'
        gotoxy(70+i,2); printf("%c",3);
    }
}

void NAVE::morir(){ // Método de clase
    if (corazones == 0){
        // Animación para hacer la exposión
        borrar(); // borra la nave

        gotoxy(x,y);   printf("        ");
        gotoxy(x,y+1); printf("   *    ");
        gotoxy(x,y+2); printf("        ");
        Sleep(200); // sleep para que se pueda notar la animación
        borrar();

        gotoxy(x,y);   printf("   **   ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf("   **   ");
        Sleep(200); // sleep para que se pueda notar la animación
        borrar();

        gotoxy(x,y);   printf(" * ** *");
        gotoxy(x,y+1); printf("  **** ");
        gotoxy(x,y+2); printf(" * ** *");
        Sleep(200); // sleep para que se pueda notar la animación
        borrar();

        vidas--; // la cantidad de vidas se disminuye
        corazones = 3; // se restaura la cantidad de corazones de la nueva vida
        pintar_corazones(); // se pinta la información de vida y corazones
        pintar(); // anteriormente se borró todo lo que concierne con la nave
                  // y su exposión por lo que de debe volver a pintar la nave
        time = 50; // reset del nivel de dificultad.
    }
}

class AST{
    int x, y;  // coordenadas
public:
    AST(int _x, int _y); // constructor
    void pintar(); // se pinta los asteriodes
    void mover(); // se mueven los asteroides
    void choque(class NAVE &N); // responde si un asteroide cocha con la nave
    /*
     * Para la interacción entre clases no se tiene la facilidad de poder acceder
     * a las coordenadas de un objeto de tipo nave, asteroide o bala
     * es por ello que los siguiente métodos retorna "el valor" de las coordenadas
     * más no ocurrirá niguna modificación de la misma de un objeto impropio.
     */
    int X(){return x;}
    int Y(){return y;}
};

AST::AST(int _x, int _y){ // Método constructor
    x = _x;
    y = _y;
}

void AST::pintar(){ // Método de clase
    // pinta el caracter que será el asteroide
    gotoxy(x,y); printf("%c",184);
}

void AST::mover(){ // Método de clase
    // primero se borra el asteroide justamente con las coordenadas que tenía antes
    gotoxy(x,y); printf(" ");
    y++; // se aumenta la coordenada y del asteroide que corresponde con el eje vertica
    if (y>32){ // si la y es mayor a 32 es porque llegó a la parte de abajo por lo tanto se resetea la coordena x
        x = (rand()%71)+4; // valor aleatorio de entre 0 y 71 con el mod, pero se aumenta en 4
                           // para que si es 0 sea 4 o si es 71 sea 75 esto responde a la coherencia de coordenadas
        y = 4; // valor default del eje y
    }
    // se vuelve a pintar el asteroide
    pintar();
}

void AST::choque(class NAVE &N){ // Método de clase
    /*
     * Este método recibe un parámetro que es un objeto de la clase NAVE
     * solo que no recibe directamente el objeto sino la referencia del mismo
     *
     * Para ello verifica con un if, que:
     *   - el valor de la x del asteroide sea mayor o igual al de la nave
     *   - también se considera el ancho(5) de la nave
     *   - el valor de la y del asteroide sea mayor o igual al de la nave
     *   - también se considera el largo (3) de la nave
     */
    if (x >= N.X() && x < N.X()+5 && y >= N.Y() && y <= N.Y()+2){
        N.COR(); // resta un corazón
        N.pintar(); // vuelve a pintar la nave
        N.pintar_corazones(); // pinta la información de vida

        x = (rand()%71)+4; // reubica al asteroide
        y = 4; // reubica al asteroide
    }
}

class BALA{
    int x, y;  // coordenadas
public:
    BALA(int _x, int _y); // constructor
    void mover(); // función para mover la bala
    bool fuera(); // bool que indica si la bala terminó su trayectoria
    /*
     * Para la interacción entre clases no se tiene la facilidad de poder acceder
     * a las coordenadas de un objeto de tipo nave, asteroide o bala
     * es por ello que los siguiente métodos retorna "el valor" de las coordenadas
     * más no ocurrirá niguna modificación de la misma de un objeto impropio.
     */
    int X(){return x;}
    int Y(){return y;}
};

BALA::BALA(int _x, int _y){ // Método constructor
    x = _x;
    y = _y;
}

void BALA::mover(){ // Método de clase
    // primer se borra la bala y después se vuelve a poner
    gotoxy(x,y); printf(" ");
    y--; // se disminuye para continuar con el trayecto que debería
    gotoxy(x,y); printf("%c",166);// se pinta la bala nuevamente
}

bool BALA::fuera(){ // Método de clase
    /*
     * Retorna True cuando la bala llegó al tope
     */
    if (y == 4){
        return true;
    }else{
        return false;
    }
}

int main()
{
    OcultarCursor();
    pintar_limites(); // pinta el marco

    // encabezado
    gotoxy(18,2); printf("Space Adventures @chapitro");
    gotoxy(2,34);printf("Use WASD or arrows to control and Space to shoot");
    gotoxy(2,36);printf("Press any key to continue");
    getch(); // espera que el usuario aprete una tecla para continuar
    gotoxy(2,36);printf("                          ");

    NAVE N(37,30,3,3); // se crea un objeto de tipo nave
    N.pintar(); // se pinta el objeto
    N.pintar_corazones(); // info de vida

    /*
     * Se crea una lista (dinámica) que contendrá cierta cantidad de asteroides
     */
    list<AST*> A; // es una lista de objetos AST pero serán punteros
    list<AST*>::iterator itA; // se crea un iterador para dicha lista
    for (int i = 0; i <4; i++){
        /*
         * el A.push_back(object); crea un objeto que será ingresado en la lista.
         * el "new AST(int _x, int _y)" sirve para crear una instancia de la clase osea un objeto
         */
        A.push_back(new AST(rand()%70+3, rand()%5+4));
    }

    list<BALA*> B; // es una lista de objetos BALA pero serán punteros
    list<BALA*>::iterator it; // se crea un iterador para dicha lista

    bool game_over = false; // condición de actividad del juego
    int puntos = 0; // cantidad de puntos

    while (!game_over){
        gotoxy(4,2); printf("Points: %d",puntos); // imprime el puntaje

        if (kbhit()){
            /*
             * Este if pretende verificar si el usuario ha apretado una tecla,
             * si corresponde con la tecla espacio ejecutará un disparo
             */
            char tcl = getch();
            if (tcl == ESPACIO)
                B.push_back(new BALA(N.X()+2, N.Y()-1)); // ejecuta un disparo
                /*
                 * B es la lista que contiene las objetos de tipo bala, por lo que se
                 * crean instancias de dicha clase con los parámetros:
                 *    N.X()+2: se obtiene la posición X y se aumenta en 2 para que parezca que sale del centro de la nave
                 *    N.Y()-1: se obtiene la posición Y y se le resta uno para que parezca que la bala viene de adentro de la nave
                 *  recuerde que aquí se "crea" la bala por lo que las posiciones deben de ser coherentes con la posición de la nave
                 */
        }

        for(it = B.begin(); it != B.end(); it++)
        {
        /*
         * Este for recorre el objeto de tipo lista que contiene los punteros de objetos de tipo BALA.
         *  B.begin() es un método del objeto lista que indica el primer elemento de la lista.
         *  B.end() es un método del objeto lista que indica el último elemento de la lista.
         *  el iterador se debe de estar aumentando.
         */

            /*
             * Recuerde que la lista B contiene punteros es por eso que la siguiente línea con el *it (iterador antes creado)
             * se accede al objeto como tal objeto de tipo BALA.
             * Después de acceder al objeto podemos acceder a sus métodos, pero es necesario el "->"
             * en vez de usar un punto. Necesitamos que cada una de las balas se empiecen a mover.
             */
            (*it)->mover();

            /*
             * Después de mover la bala debemos verificar si esta llegó al límite, el métod fuera() nos dice eso.
             * si llegó al límite se borra la bala, se 'elimina el objeto de la lista pero 'enlaza al siguiente para no tener problemas
             */
            if ((*it)->fuera()){// se verifica si ya llegó al punto más alto posible y se borra
                gotoxy((*it)->X(),(*it)->Y()); printf(" "); // se elimina de la pantalla
                delete (*it); // se elimina de la lista
                it = B.erase(it); // es una lista si anteriormente se elimina el elemento se interrupme la secuencia
                                  // por lo que esto como que enlaza al siguiente
            }
        }


        for (itA = A.begin(); itA != A.end(); itA++)
        {
        /*
         * Este for recorre el objeto de tipo lista que contiene los punteros de objetos de tipo AST-eroides.
         *  A.begin() es un método del objeto lista que indica el primer elemento de la lista.
         *  A.end() es un método del objeto lista que indica el último elemento de la lista.
         *  el iterador se debe de estar aumentando.
         */

            /*
             * Recuerde que la lista A contiene punteros es por eso que la siguiente línea con el *itA (iterador antes creado)
             * se accede al objeto como tal objeto de tipo AST.
             * Después de acceder al objeto podemos acceder a sus métodos, pero es necesario el "->"
             * en vez de usar un punto. Recurrimos a dos métodos necesarios para el funcionamiento de los asteroides.
             * Primero se mueven los asteroides y después se verifica si ha ocurrido un choque CON LA NAVE.
             */
            (*itA)->mover();
            (*itA)->choque(N); // Se pasa como parámetro el objeto de tipo NAVE N
        }



        /*
         * Este último for recorrerá todos los AST-eroides pero por cada uno recorrerá a su vez todos las BALA-s.
         * Cuando se recorre cada una de las balas, es cuando se verifica si ha colisionado una bala con un asteroide.
         */
        for (itA = A.begin(); itA != A.end(); itA++)
        {
            for (it = B.begin(); it != B.end(); it++)
            {
                /*
                 * Existen potencialmente dos situaciones para que una bala colisione a un asteoroide.
                 *  - Las posiciones XY del asteroide y la bala son iguales.
                 *  - Que la posición Y+1 del asteroide sea igual a la posición Y de la bala.
                 *
                 * Pero ante todo esto lo impresindible es que las posiciones X del asteroide y de la bala sean iguales.
                 */
                if((*itA)->X() == (*it)->X() && ( (*itA)->Y()+1 == (*it)->Y() || (*itA)->Y() == (*it)->Y()) ){
                    gotoxy((*it)->X(), (*it)->Y()); printf(" "); // se borra la bala
                    delete (*it); // se elimina el objeto de la lista
                    it = B.erase(it); // se enlaza al siguiente objeto para no tener inconvenientes

                    A.push_back(new AST(rand()%70+3, 4)); // se agrega un objeto de tipo asteroide para recuperar que se borrará
                    gotoxy((*itA)->X(), (*itA)->Y()); printf(" "); // se borra el asteroide
                    delete (*itA); // se elimina el objeto de la lista
                    itA = A.erase(itA); // se enlaza al siguiente objeto para no tener inconvenientes

                    puntos += 5; // aumenta la cantidad de punto
                    if (time>20)
                    {
                    // disminuye el time para que la dificultad (rapidez y numero de asteroides del juego aumente)
                        time -=3;
                        A.push_back(new AST(rand()%70+3, 4)); // se crea otro asteroide
                    }
                }
            }
        }


        if (N.VIDAS() == 0) // validación del final del juego
            game_over = true;

        N.morir(); // constante verificación de si la nave se ha destruido
        N.mover(); // constante posibilidad de que se mueva la nave
        Sleep(time);// 30 milisegundos para evitar sobre cargar con el while siempre en true
    }


    for(int i = 0; i<80; i++){ // columnas
        for (int j = 0; j <36; j++){ // filas
            gotoxy(i,j);printf("."); // borra la pantalla por puntos.
            Sleep(1); // para que se pueda observar el cambio.
        }
    }

    gotoxy(30,38);printf("¡GAME OVER!"); // msj final

    getch(); // mantiene abierta la pantalla hasta que el usuario aprete una tecla y se cierre la ventana
    return 0;
}
