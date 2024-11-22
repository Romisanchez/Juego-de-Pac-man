#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define FILAS 15
#define COLUMNAS 30

char mapa[FILAS][COLUMNAS] = {
    "##############################",
    "#............#...............#",
    "#.####.#####.#.######.######.#",
    "#.#  #.#   #.#.#    #.#    #.#",
    "#.#  #.#   #.#.#    #.#    #.#",
    "#.####.#####.#.######.######.#",
    "#............................#",
    "#.####.#####.###.##########.##",
    "#....#.#   #.....#...........#",
    "#.####.#####.#####.###########",
    "#..............#.............#",
    "###.#####.#####.##########.###",
    "#...#.....#.........#........#",
    "#.####.###########.#.######.##",
    "##############################"};


int pacmanX = 1, pacmanY = 1;
int fantasmaX1 = 8, fantasmaY1 = 15; 
int fantasmaX2 = 8, fantasmaY2 = 14; 
int fantasmaX3 = 8, fantasmaY3 = 16; 

int vidas = 3;
int puntaje = 0;
int turno = 0;
int fantasmasLiberados = 0;

const int JAULA_X = 8;
const int JAULA_Y1 = 15;
const int JAULA_Y2 = 14;
const int JAULA_Y3 = 16;

void cambiarColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void sonidoInicio() {
    Beep(600, 200);
    Beep(800, 200);
    Beep(1000, 200);
}

void pantallaInicio() {
    system("cls");
    cambiarColor(14);
    printf("\n\n");
    printf(" ########     ###     ########       ##       ##     ###    ##    ##    \n");
    printf(" ##    ##  ##     ##  ##             ###     ###  ##     ## ###   ##    \n");
    printf(" ##    ##  ##     ##  ##             ####   ####  ##     ## ####  ##    \n");
    printf(" ########  #########  ##       ####  ## ## ## ##  ######### ## ## ##    \n");
    printf(" ##        ##     ##  ##             ##   #   ##  ##     ## ##  ####    \n");
    printf(" ##        ##     ##  ##             ##       ##  ##     ## ##   ###    \n");
    printf(" ##        ##     ##  ########       ##       ##  ##     ## ##    ##    \n");
    Sleep(1500);
    system("cls");
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void imprimirMapa() {
    system("cls");
    printf("Vidas: %d | Puntaje: %d | ", vidas, puntaje);
    if (!fantasmasLiberados) {
        printf("Fantasmas liberados en: %d segundos\n", 5 - (int)(clock() / 1000));
    } else {
        printf("¡Fantasmas liberados!\n");
    }
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == pacmanX && j == pacmanY) {
                cambiarColor(14);
                printf(turno % 2 == 0 ? "C" : "O");
            } else if (i == fantasmaX1 && j == fantasmaY1) {
                cambiarColor(4);
                printf("G");
            } else if (i == fantasmaX2 && j == fantasmaY2) {
                cambiarColor(2);
                printf("G");
            } else if (i == fantasmaX3 && j == fantasmaY3) {
                cambiarColor(9);
                printf("G");
            } else if (mapa[i][j] == '#') {
                cambiarColor(13);
                printf("#");
            } else if (mapa[i][j] == '.') {
                cambiarColor(7);
                printf(".");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void moverPacman(char direccion) {
    int nuevoX = pacmanX;
    int nuevoY = pacmanY;
    
    if (direccion == 'w') {
        if (mapa[pacmanX - 1][pacmanY] != '#') {
            nuevoX--;
            if (mapa[pacmanX - 2][pacmanY] != '#') nuevoX--;
        }
    }
    if (direccion == 's') {
        if (mapa[pacmanX + 1][pacmanY] != '#') {
            nuevoX++;
            if (mapa[pacmanX + 2][pacmanY] != '#') nuevoX++;
        }
    }
    if (direccion == 'a') {
        if (mapa[pacmanX][pacmanY - 1] != '#') {
            nuevoY--;
            if (mapa[pacmanX][pacmanY - 2] != '#') nuevoY--;
        }
    }
    if (direccion == 'd') {
        if (mapa[pacmanX][pacmanY + 1] != '#') {
            nuevoY++;
            if (mapa[pacmanX][pacmanY + 2] != '#') nuevoY++;
        }
    }

    if (mapa[nuevoX][nuevoY] != '#') {
        pacmanX = nuevoX;
        pacmanY = nuevoY;

        if (mapa[pacmanX][pacmanY] == '.') {
            puntaje += 10;
            mapa[pacmanX][pacmanY] = ' ';
            Beep(500, 100);
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------
void moverFantasma(int *x, int *y, int velocidad) {
    if (!fantasmasLiberados) {
        return;
    }

    int dx = 0, dy = 0;
    
    if (pacmanX > *x) dx = 1;
    if (pacmanX < *x) dx = -1;
    if (pacmanY > *y) dy = 1;
    if (pacmanY < *y) dy = -1;

    if (mapa[*x + dx][*y] != '#') *x += dx;
    if (mapa[*x][*y + dy] != '#') *y += dy;

    if (*x == pacmanX && *y == pacmanY) {
        vidas--;
        pacmanX = 1; 
        pacmanY = 1;
        Beep(300, 300);
    }

    Sleep(velocidad);
}

void reiniciarJuego() {
    vidas = 3;
    puntaje = 0;
    fantasmaX1 = JAULA_X;
    fantasmaY1 = JAULA_Y1;
    fantasmaX2 = JAULA_X;
    fantasmaY2 = JAULA_Y2;
    fantasmaX3 = JAULA_X;
    fantasmaY3 = JAULA_Y3;
    fantasmasLiberados = 0;
    pacmanX = 1;
    pacmanY = 1;
    system("cls");
    juego();
}

void preguntarReiniciar() {
    char respuesta;
    printf("Quieres jugar nuevamente? (s/n): ");
    scanf(" %c", &respuesta);
    if (respuesta == 's' || respuesta == 'S') {
        reiniciarJuego();
    } else {
        printf("Gracias por jugar. ¡Hasta la próxima!\n");
        exit(0);
    }
}

void juego() {
    pantallaInicio();
    sonidoInicio();

    clock_t startTime = clock();

    while (vidas > 0) {
        imprimirMapa();
        if ((clock() - startTime) >= 5000 && !fantasmasLiberados) {
            fantasmasLiberados = 1;
            Beep(1000, 500); // 
        }

        if (_kbhit()) {
            char tecla = _getch();
            moverPacman(tecla);
        }

        int velocidadFantasmas = 200; 
        moverFantasma(&fantasmaX1, &fantasmaY1, velocidadFantasmas);
        moverFantasma(&fantasmaX2, &fantasmaY2, velocidadFantasmas);
        moverFantasma(&fantasmaX3, &fantasmaY3, velocidadFantasmas);

        if (vidas == 0) {
            printf("¡Perdiste todas tus vidas!\n");
            preguntarReiniciar();
        }

        turno++;
        Sleep(50); 
    }
}

int main() {
    juego();
    return 0;
}