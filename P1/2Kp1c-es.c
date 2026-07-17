/**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas las funciones que debéis 
 * implementar en ensamblador.
 * Desde este código se realizan las llamadas a las subrutinas de ensamblador. 
 * ESTE CÓDIGO NO SE PUEDE MODIFICAR Y NO SE DEBE ENTREGAR.
 **/
 
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

/**
 * Constantes.
 **/
#define DIMMATRIX  4     //dimensió de la matriu
#define SIZEMATRIX DIMMATRIX*DIMMATRIX //=16


/**
 * Definición de variables globales.
 **/
extern int developer;//Variable declarada en assemblador que indica el nom del programador

int  rowScreen;//Fila donde posicionar el cursor en la pantalla.
int  colScreen;//Columna donde posicionar el cursor en la pantalla.
char charac;   //Caràcter llegit de teclat i per a escriure a pantalla.

// Matriz 4x4 donde guardaremos los números del juego
// Acceso a las matrices en C: utilizamos fila (0..[DIMMATRIX-1]) y
// columna(0..[DIMMATRIX-1]) (m[fila][columna]).
// Acceso a las matrices en ensamblador: Se accede a ella como si fuera un vector
// donde indexMat (0..[DIMMATRIX*DIMMATRIX-1]).
// indexMat=((fila*DIMMATRIX)+(columna))*4 (4 porque la matriz es de tipo int).
// DWORD[M+indexMat] (DWORD porque es de tipo int)
// (indexMat debe ser un registro de tipo long/QWORD:RAX,RBX,..,RSI,RDI,..,R15).
int   m[DIMMATRIX][DIMMATRIX]        = { {    8,    8,    32,    32},
                                         {    4,   32,   128,    64},
                                         {    0,    0,   256,   128},
                                         {    0,    4,   512,  1024} };

int   mRotated[DIMMATRIX][DIMMATRIX] = { {    2,    0,     2,     0},
                                         {    2,    2,     4,     4},
                                         {    4,    4,     0,     4},
                                         {    4,    2,     2,     4} };

short state  = 1;   // Estado del juego.
                    // 0: Salir, hemos pulsado la tecla 'ESC' para salir.
                    // 1: Continuemos jugando.
                    // 2: Continuamos jugando pero se han hecho cambios en la matriz.

long  number;          //Número que queremos mostrar.
long  score  = 290500; //Puntos acumulados en el marcador.                    
char  dir;             //'L' rotación a la izquierda, 'R' rotación a la derecha.

/**
 * Definición de las funciones en C.
 **/
void clearscreen_C();
void gotoxyP1_C();
void printchP1_C();
void getchP1_C();

void printMenuP1_C();
void printBoardP1_C();

void showNumberP1_C();
void updateBoardP1_C();
void copyMatrixLRP1_C();
void rotateMatrixRP1_C();
void shiftNumbersRP1_C();
void addPairsRP1_C();

void insertTileP1_C();

void printMessageP1_C();
void playP1_C();


/**
 * Definición de las subrutinas en ensamblador que se llaman desde C.
 **/
extern void  showNumberP1();
extern void  updateBoardP1();
extern void  copyMatrixP1();
extern void  rotateMatrixLRP1();
extern void  shiftNumbersLP1();
extern void  addPairsLP1();
extern void  checkEndP1();
extern void  playP1();


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:   
 * Ninguna.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 **/
void clearScreen_C(){
   
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en la fila indicada por la variable (rowScreen) y en 
 * la columna indicada por la variable (colScreen) de la pantalla.
 * 
 * Variables globales utilizadas:   
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'gotoxyP1' para 
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void gotoxyP1_C(){
   
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter guardado en la variable (charac) en la pantalla, 
 * en la posición donde está el cursor.
 * 
 * Variables globales utilizadas:   
 * (charac): Carácter que queremos mostrar.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'printchP1' para
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void printchP1_C(){

   printf("%c",charac);
   
}


/**
 * Leer una tecla y guardar el carácter asociado en la variable (charac)
 * sin mostrarlo en pantalla. 
 * 
 * Variables globales utilizadas:   
 * (charac): Carácter que leemos de teclado.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'getchP1' para
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void getchP1_C(){

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándar (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

   /* ~ICANON para tratar la entrada del teclado carácter a carácter no como una línea completa acabada con /n
      ~ECHO para que no muestre el carácter leído*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente. **/
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   charac = (char) getchar();                 
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
   
}


/**
 * Mostrar en la pantalla el menú del juego y pedir una opción.
 * Solo acepta una de las opciones correctas del menú ('0'-'9') o ESC.
 * 
 * Variables globales utilizadas:   
 * (developer): ((char;)&developer): variable definida en el código ensamblador.
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * (charac)   : Opción elegida del menú, leída de teclado.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 **/
void printMenuP1_C(){
    clearScreen_C();
    rowScreen = 1;
    colScreen = 1;
    gotoxyP1_C();
    printf("                                    \n");
    printf("           Developed by:            \n");
    printf("        ( %s )   \n",(char *)&developer);
    printf(" __________________________________ \n");
    printf("|                                  |\n");
    printf("|         MAIN MENU 1.0            |\n");
    printf("|__________________________________|\n");
    printf("|                                  |\n");
    printf("|         1. ShowNumber            |\n");
    printf("|         2. UpdateBoard           |\n");
    printf("|         3. CopyMatrix            |\n");
    printf("|         4. RotateMatrix          |\n");
    printf("|         5. ShiftNumbers          |\n");
    printf("|         6. AddPairs              |\n");
    printf("|         7. -(new in P2)-         |\n");
    printf("|         8. Play Game             |\n");
    printf("|         9. Play Game C           |\n");
    printf("|         0. Exit                  |\n");
    printf("|__________________________________|\n");
    printf("|                                  |\n");
    printf("|            OPTION:               |\n");
    printf("|__________________________________|\n"); 

    charac=' ';
    while (charac < '0' || charac > '9') {
      rowScreen = 21;
      colScreen = 22;
      gotoxyP1_C();
      getchP1_C();
      printchP1_C();
   }
   
}


/**
 * Mostrar el tablero de juego en la pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:	
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 */
void printBoardP1_C(){

   rowScreen = 1;
   colScreen = 1;
   gotoxyP1_C();                                                     //rowScreen
   printf(" _________________________________________________  \n"); //01
   printf("|                                                  |\n"); //02
   printf("|                  2048 PUZZLE  v1.0               |\n"); //03
   printf("|                                                  |\n"); //04
   printf("|     Join the numbers and get to the 2048 tile!   |\n"); //05
   printf("|__________________________________________________|\n"); //06
   printf("|                                                  |\n"); //07
   printf("|            0        1        2        3          |\n"); //08
   printf("|        +--------+--------+--------+--------+     |\n"); //09
   printf("|      0 |        |        |        |        |     |\n"); //10
   printf("|        +--------+--------+--------+--------+     |\n"); //11
   printf("|      1 |        |        |        |        |     |\n"); //12
   printf("|        +--------+--------+--------+--------+-    |\n"); //13
   printf("|      2 |        |        |        |        |     |\n"); //14
   printf("|        +--------+--------+--------+--------+     |\n"); //15
   printf("|      3 |        |        |        |        |     |\n"); //16
   printf("|        +--------+--------+--------+--------+     |\n"); //17
   printf("|                   Score:   ______                |\n"); //18
   printf("|__________________________________________________|\n"); //19
   printf("|                                                  |\n"); //20
   printf("|  (ESC)Exit  (i)Up   (j)Left  (k)Down  (l)Right   |\n"); //21
   printf("|__________________________________________________|\n"); //22
   
}


/**
 * Convierte el número de la variable (number) de tipo long de 6 dígitos 
 * (0 <= number <= 999999) a caracteres ASCII que representan su valor. 
 * Si (num) es menor que 0 cambiamos el valor a 0, 
 * si es mayor que 999999 cambiaremos el valor a 999999. 
 * Se debe dividir(/) el valor entre 10, de forma iterativa, 
 * hasta obtener el 6 dígitos. 
 * En cada iteración, el residuo de la división (%) que es un valor 
 * entre (0-9) indica el valor del dígito a convertir 
 * a ASCII ('0' - '9') sumando '0' (48 decimal) para poder mostrarlo. 
 * Cuando el cociente sea 0 mostraremos espacios en la parte no significativa. 
 * Por ejemplo, si (number)=103 mostraremos "   103" y no "000103". 
 * Se deben mostrar los dígitos (carácter ASCII) desde la posición 
 * indicada por las variables (rowScreen) y (colScreen) 
 * posición de las unidades, hacia la izquierda. 
 * El primer dígito que obtenemos son las unidades, después las decenas, 
 * ..., para mostrar el valor se debe desplazar el cursor una posición 
 * a la izquierda en cada iteración. 
 * Para posicionar el cursor se llama a la función gotoxyP1_C y para 
 * mostrar los caracteres en la función printchP1_C.
 * 
 * Variables globales utilizadas:   
 * (number)   : Número que queremos mostrar.
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * (charac)   : caràcter a escribri en pantalla.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'showNumberP1'. 
 */
 void showNumberP1_C() {
   
   long num = number;
   int i;

   if (num < 0) num = 0;
   if (num > 999999) num = 999999;
   for (i=0;i<6;i++){
     charac = ' ';
     if (num > 0) {
	   charac = (char)(num%10);    //residuo
	   num = num/10;               //cociente
	   charac = charac + '0';
	 }
     gotoxyP1_C();
     printchP1_C();
     colScreen--;
   }

}


/**
* Actualizar el contenido del Tablero de Juego con los datos de la 
* matriz (m) de 4x4 de tipo int y los puntos del marcador 
* (score) que se han hecho. 
* Se debe recorrer toda la matriz (m), y para cada elemento de la matriz 
* posicionar el cursor en la pantalla y mostrar el número de aquella 
* posición de la matriz. 
* Recorrer toda la matriz por filas de izquierda a derecha y de arriba abajo. 
* Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0]) 
* a 60 (posición [3][3]) con incrementos de 4 porque los datos son de 
* tipo int(DWORD) 4 bytes. 
* Luego, mostrar el marcador (score) en la parte inferior del tablero, 
* fila 18, columna 35 llamando la función showNumberP1_C. 
* Finalmente posicionar el cursor en la fila 18, columna 36 llamando la 
* función gotoxyP1_C(). 
* 
* Variables globales utilizadas: 
* (rowScreen): Fila de la pantalla donde posicionamos el cursor. 
* (colScreen): Columna de la pantalla donde posicionamos el cursor. 
* (m)        : Matriz donde guardamos los números del juego. 
* (score)    : Puntos acumulados en el marcador. 
* (number)   : Número que queremos mostrar.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'updateBoardP1'.
 */
void updateBoardP1_C(){

   int i,j;
   int rowScreenAux;
   int colScreenAux;
   
   rowScreenAux = 10;
   for (i=0;i<DIMMATRIX;i++){
     colScreenAux = 17;
      for (j=0;j<DIMMATRIX;j++){
         number = (long)m[i][j];
         rowScreen = rowScreenAux;
         colScreen = colScreenAux;
         showNumberP1_C();
         colScreenAux = colScreenAux + 9;
      }
      rowScreenAux = rowScreenAux + 2;
   }
   
   number = score;
   rowScreen = 18;
   colScreen = 35;
   showNumberP1_C();   
   rowScreen = 18;
   colScreen = 36;
   gotoxyP1_C();
   
}


/**
 * Copiar los valores de la matriz (mRotated) en la matriz (m). 
 * La matriz (mRotated) no debe modificarse, 
 * los cambios deben realizarse en la matriz (m). 
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba abajo. 
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0]) 
 * a 60 (posición [3][3]) con incrementos de 4 porque los datos son de 
 * tipo int(DWORD) 4 bytes. 
 * No se muestrar la matriz. 
 * 
 * Variables globales utilizadas: 
 * (m)       : Matriz donde guardamos los números del juego. 
 * (mRotated): Matriz con los números rotados.
 *
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'copyMatrixP1'.
 */
void copyMatrixP1_C() {

   int i,j;
   
   for (i=0; i<DIMMATRIX; i++) {
      for (j=0; j<DIMMATRIX; j++) {   
         m[i][j] = mRotated[i][j];
      }
   }
   
}


/**
 * Rotar a la derecha o a la izquierda la matriz (m) en la dirección 
 * indicada por la variable (dir: 'L' izquierda, 'R' derecha) sobre la 
 * matriz (mRotated). 
 * Si se rota a la izquierda (dir='L') 
 * la primera fila pasa a ser la cuarta columna, 
 * la segunda fila pasa a ser la tercera columna, 
 * la tercera fila pasa a ser la segunda columna y 
 * la cuarta fila pasa a ser la primera columna. 
 * Si se rota a la derecha (dir='R') 
 * la primera columna pasa a ser la cuarta fila, 
 * la segunda columna pasa a ser la tercera fila, 
 * la tercera columna pasa a ser la segunda fila y 
 * la cuarta columna pasa a ser la primera fila. 
 * En el enunciado se explica en más detalle cómo hacer la rotación. 
 * NOTA: NO es lo mismo que hacer la matriz transpuesta. 
 * La matriz (m) no debe modificarse, 
 * los cambios deben realizarse en la matriz (mRotated). 
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0]) 
 * a 60 (posición [3][3]) con incrementos de 4 porque los datos son de 
 * tipo int(DWORD) 4 bytes. 
 * Para acceder a una posición concreta de la matriz desde ensamblador 
 * hay que tener en cuenta que el índice es:(index=(fila*DIMMATRIX+columna)*4), 
 * multiplicamos por 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * Una vez realizada la rotación, copiar la matriz (mRotated) en la matriz 
 * (mToRotate) llamando la función copyMatrixP1_C(). 
 * No se debe mostrar la matriz. 
 * 
 * Variables globales utilizadas: 
 * (m)       : Matriz donde guardamos los números del juego. 
 * (mRotated): Matriz con los números rotados. 
 * (dir)     : Dirección de la rotación: 'L' izquierda, 'R' derecha.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'rotateMatrixLRP1'.
 */
void rotateMatrixLRP1_C() {
   
   int i,j;
   
   for (i=0; i<DIMMATRIX; i++) {
      for (j=0; j<DIMMATRIX; j++) {
         if(dir=='L') mRotated[DIMMATRIX-1-j][i] = m[i][j];
         if(dir=='R') mRotated[j][DIMMATRIX-1-i] = m[i][j];
      }
   }
   
   copyMatrixP1_C();
   
}


/**
 * Desplaza los números de cada fila de la matriz (m) a la izquierda
 * manteniendo el orden de los números y 
 * colocando ceros a la derecha si se desplaza.
 * Recorre la matriz por filas de izquierda a derecha y de arriba a abajo.
 * Para recorrer la matriz en ensamblador, el índice va de
 * la posición 0 (posición [0][0]) a la 60 (posición [3][3]) con incrementos 
 * de 4, ya que los datos son de tipo int(DWORD) de 4 bytes.
 * Para acceder a una posición específica de la matriz desde ensamblador,
 * es necesario tener en cuenta que el índice es: (índice=(fila*DIMATRIX+columna)*4),
 * se multiplica por 4, ya que los datos son de tipo int(DWORD) de 4 bytes. 
 * Si se desplaza un número (NO LOS CEROS), los desplazamientos deben 
 * contarse incrementando la variable (desplazamientos).
 * En cada fila, si encuentra un 0, busca un número distinto de cero 
 * en la misma fila para colocarlo en esa posición.
 * Si una fila de la matriz es: [0,2,0,4], será [2,4,0,0] y (estado = 2).
 * Los cambios deben realizarse en la misma matriz.
 * La matriz no debe mostrarse.
 *
 * Variables globales utilizadas:
 * (m)     : Matriz donde se almacenan los números de la partida.
 * (estado): Estado del juego. (2: Se han realizado movimientos).
 *
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'shiftNumbersLP1', 
 */
void shiftNumbersLP1_C() {
   
   int i,j,k;
   
   for (i=0; i<DIMMATRIX; i++) {
	 for (j=0; j<DIMMATRIX-1; j++) {
        if (m[i][j] == 0) {
          k = j+1;           
          while (k<DIMMATRIX && m[i][k]==0) k++;
          if (k<DIMMATRIX) {
            m[i][j]= m[i][k];
            m[i][k]= 0;
            state = 2;      
          }
        }
      }
    }
    
}
   

/**
 * Emparejar números de la matriz (m) hacia la izquierda y acumular los 
 * puntos (p) en el marcador sumando los puntos de las parejas que se hayan hecho. 
 * Recorrer la matriz por filas de izquierda a derecha y de arriba a abajo.
 * Cuando se encuentre una pareja, dos casillas consecutivas de la misma 
 * fila con el mismo número, juntamos la pareja poniendo la suma de la 
 * pareja en la casilla de la derecha, un 0 en la casilla de la izquierda y 
 * acumularemos esta suma en la variable (p) (puntos ganados). 
 * Si una fila de la matriz es: : [4,4,2,2] quedará [8,0,4,0] y 
 * p = p + (8+4). 
 * Si al final se ha juntado alguna pareja (p>0), pondremos la variable 
 * (state) a 2 para indicar que se ha movido algún número y actualizaremos 
 * la variable (score) con los puntos obtenidos de hacer las parejas. 
 * Para recorrer la matriz en ensamblador, el índice va de la 
 * posición 0 (posición [0][0]) a la 60 (posición [3][3]) con incrementos 
 * de 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * Para acceder a una posición concreta de la matriz desde ensamblador 
 * hay que tener en cuenta que el índice es:(index=(fila*DimMatrix+columna)*4), 
 * multiplicamos por 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * Los cambios deben realizarse sobre la misma matriz. 
 * No se debe mostrar la matriz. 
 * 
 * Variables globales utilizadas: 
 * (m)    : Matriz donde guardamos los números del juego. 
 * (score): Puntos acumulados en el marcador. 
 * (state): Estado del juego. (2: Se han hecho alguna pareja).
 *
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'addPairsLP1'.
 */
void addPairsLP1_C() {
   
   int i,j;
   int p = 0;
   
  for (i=0; i<DIMMATRIX; i++) {
     for (j=0; j<DIMMATRIX-1; j++) {
       if ((m[i][j]!=0) && (m[i][j]==m[i][j+1])) {
         m[i][j]  = m[i][j]*2;
         m[i][j+1]= 0;
         p = p + m[i][j];
         j++;
       }		
     }
   }
   
   if (p > 0) {
      score = score + (long)p;
      state = 2;
   }
   
}


/**
 * Generar nueva ficha de forma aleatoria. 
 * Si hay al menos una casilla vacía en la matriz (m) genera una 
 * fila y una columna de forma aleatoria hasta que es una de las casillas 
 * vacías. A continuación pone un 2 en esa casilla. 
 * 
 * Variables globales utilizadas: 
 * (m): Matriz donde guardamos los números del juego.
 * 
 * Aquesta funció es crida des de C i des d'assemblador,
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
void insertTileP1_C() {
   
   int i,j,k,l;
   
   i=DIMMATRIX; // Miramos si hay una casilla vacia.
   do {
      i--;
      j=DIMMATRIX;
      do {
         j--;   
      } while ((j>0) && (m[i][j]!=0));
   } while ((i>0) && (m[i][j]!=0));
   
   //Insetamos el 2 si hay com a mínim una casilla vacia.
   if (m[i][j]==0) { 
      do { // Genera filas y columnas aleatoriamente hasta que encuentra
           // una posición vacia.
         k = rand() % 4; l = rand() % 4; }
      while( m[k][l] != 0 );
      
      m[k][l] = 2; //Ponemos un 2.
      
   }
      
}


/**
 * Mostrar un mensaje debajo del tablero según el valor de la variable 
 * (state).
 * state: 0: Hemos pulsado la tecla 'ESC' para salir del juego.
 * Se espera que se pulse una tecla para continuar.
 *  
 * Variables globales utilizadas:	
 * (rowScreen) : Fila de la pantalla donde se situa el cursor.
 * (colScreen) : Columna de la pantalla donde se situa el cursor.
 * (state)     : Estado del juego. 
 *  
 * Aquesta funció es crida des de C i des d'assemblador,
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
void printMessageP1_C() {
 
   rowScreen = 23;
   colScreen = 12;
   gotoxyP1_C();
 
   switch(state){
      case 0:
         printf("<<<<<< EXIT: (ESC) Pressed >>>>>>");
      break;
   }
   getchP1_C();
   
}
 

/**
 * Juego del 2048 
 * Función principal del juego 
 * Permite jugar al juego del 2048 llamando a todas las funcionalidades. 
 * 
 * Pseudo código: 
 * Inicializar estado del juego, (state=1) 
 * Mostrar el tablero de juego (llamar la función printBoardP1_C). 
 * Actualiza el contenido del Tablero de Juego y los puntos que se han hecho 
 * (llamar la función updateBoardP1_C). 
 * Mientras (state==1) hacer 
 * Leer una tecla (charac) llamando la función getchP1_C. 
 * Según la tecla leída llamaremos a las funciones correspondientes. 
 * ['i' (arriba),'j'(izquierda),'k' (abajo) o 'l'(derecha)] 
 * Rotar la matriz (m) llamando la función rotateMatrixLRP1_C, para 
 * poder hacer los desplazamientos de los números a la izquierda llamando la 
 * función shiftNumbersLP1_C y hacer las parejas llamando la función 
 * addPairsLP1_C que también actualiza los puntos que se han sumado al 
 * marcador (score), volver a desplazar los números a la izquierda 
 * llamando a la función shiftNumbersLP1_C con las parejas hechas. 
 * Si se ha hecho algún desplazamiento o alguna pareja (state=2). 
 * Luego, volver a rotar llamando la función rotateMatrixLRP1_C 
 * para dejar la matriz en la posición inicial. 
 * Para la tecla 'j' (izquierda) no es necesario hacer rotaciones, 
 * para el resto se deben hacer rotaciones. 
 * '<ESC>' (ASCII 27) poner (state = 0) para salir del juego. 
 * Si no es ninguna de estas teclas no hacer nada. 
 * Si se ha hecho algún desplazamiento o alguna pareja (state==2), generar 
 * una nueva ficha llamando la función insertTileP1_C y poner 
 * la variable state a 1 (state=1). 
 * Actualiza el contenido del Tablero de Juego y los puntos que se han hecho 
 * llamar a la función updateBoardP1_C. 
 * Fin mientras. 
 * Mostrar un mensaje debajo del tablero según el valor de la variable 
 * (state) llamando la función printMessageP1_C. 
 * Salir: 
 * Se terminado el juego. 
 * 
 * Variables globales utilizadas: 
 * (charac): Carácter que leemos de teclado. 
 * (state) : Estado del juego. 
 *           0 : Salir, hemos pulsado la tecla 'ESC'). 
 *           1 : Continuamos jugando. 
 *           2 : Continuamos jugando pero se han hecho cambios en la matriz. 
 * (dir)   : Dirección de la rotación: 'L' izquierda, 'R' derecha.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente playP1
 */
void playP1_C(){
   
   state = 1;               
   
   printBoardP1_C();
   updateBoardP1_C();       
   while (state == 1) {
      
     getchP1_C();
      
     switch(charac){
       case 'i': //i:(105) up
         dir = 'L';
         rotateMatrixLRP1_C();
		 
         shiftNumbersLP1_C();
         addPairsLP1_C();
         shiftNumbersLP1_C();
         
         dir = 'R';
         rotateMatrixLRP1_C();
       break;
       case 'j': //j:(106) left
         shiftNumbersLP1_C();
         addPairsLP1_C();
         shiftNumbersLP1_C();
       break;
       case 'k': //k:(107) down
         dir = 'R';
         rotateMatrixLRP1_C();
             
         shiftNumbersLP1_C();
         addPairsLP1_C(m);
         shiftNumbersLP1_C();
         
		 dir = 'L';
         rotateMatrixLRP1_C();
       break;
       case 'l': //l:(108) right
         dir = 'R';
         rotateMatrixLRP1_C();
         rotateMatrixLRP1_C();
       
         shiftNumbersLP1_C();
         addPairsLP1_C(m);
         shiftNumbersLP1_C();
         
         dir = 'L';
         rotateMatrixLRP1_C();
         rotateMatrixLRP1_C();
       break;
	   case 27: //ESC:(27) exit
		 state = 0;
	   break;
	 }
      
     if (state == 2) {     
       insertTileP1_C();    
       state = 1;
     }
     updateBoardP1_C();
   }
   printMessageP1_C();        
   
}


/**
 * Programa Principal
 * 
 * ATENCIÓ: Podeu provar la funcionalitat de les subrutines que s'han de
 * desenvolupar treient els comentaris de la crida a la funció 
 * equivalent implementada en C que hi ha sota a cada opció.
 * Per al joc complet hi ha una opció per la versió en assemblador i 
 * una opció pel joc en C.
 */
int main(void){
   
   while (charac!='0') {
     clearScreen_C();
     printMenuP1_C();    
      
      switch(charac){
         case '1':// Mostrar puntos
            clearScreen_C();  
            printBoardP1_C(); 
            rowScreen = 25;
            colScreen = 20;
            gotoxyP1_C();
            printf(" Press any key ");
            rowScreen = 18;
            colScreen = 35;
            number = score;
            //=======================================================
            showNumberP1(); 
            ///showNumberP1_C();   
            //=======================================================
            getchP1_C();
         break;
         case '2': //Actualitar el contenido del tablero.
            clearScreen_C();  
            printBoardP1_C(); 
            //=======================================================
            updateBoardP1(); 
            ///updateBoardP1_C();  
            //=======================================================
            rowScreen = 25;
            colScreen = 20;
            gotoxyP1_C();
            printf(" Press any key ");
            getchP1_C();
         break;
         case '3': //Copiar matriz mRotated a m
            clearScreen_C(); 
            printBoardP1_C();
            //===================================================
            copyMatrixP1();
            ///copyMatrixP1_C();
            //===================================================
            updateBoardP1_C();
            rowScreen = 25;
            colScreen = 20;
            gotoxyP1_C();
            printf(" Press any key ");
            getchP1_C();
         break;
         case '4': //Rotar matriz
            clearScreen_C();  
            printBoardP1_C();
            dir = 'L'; 
            //===================================================
            rotateMatrixLRP1();
            ///rotateMatrixLRP1_C();
            //===================================================
            updateBoardP1_C();
            rowScreen = 25;
            colScreen = 20;
            gotoxyP1_C();
            printf(" Press any key ");
            getchP1_C();
         break;
         case '5': //Desplaza números
            clearScreen_C();  
            printBoardP1_C();
            //===================================================
            shiftNumbersLP1(); 
            ///shiftNumbersLP1_C(); 
            //===================================================
            updateBoardP1_C(); 
            rowScreen = 25;
            colScreen = 20;
            gotoxyP1_C();
            printf(" Press any key ");
            getchP1_C();
         break;
         case '6': //Hacer parejas y sumar puntos
            clearScreen_C();  
            printBoardP1_C(); 
            //===================================================
            addPairsLP1();
            ///addPairsLP1_C();
            //===================================================
            updateBoardP1_C();
            rowScreen = 25;
            colScreen = 20;
            gotoxyP1_C();
            printf(" Press any key ");
            getchP1_C();
         break;
         case '8': //Juego completo en Ensamblador    
            clearScreen_C();
            //=======================================================
            playP1();
            //=======================================================
         break;
         case '9': //Juego complet en C    
            clearScreen_C();
            //=======================================================
            playP1_C();
            //=======================================================
         break;
      }
   }
   printf("\n\n");
   
   return 0;
}
