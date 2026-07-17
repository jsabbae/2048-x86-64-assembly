/**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas las funciones que 
 * debéis implementar en ensamblador.
 * Desde este código se hacen las llamadas a las subrutinas de ensamblador.
 * ESTE CÓDIGO NO SE PUEDE MODIFICAR Y NO SE DEBE ENTREGAR.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>     //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>      //STDIN_FILENO


/**
 *  Constantes.
 */
#define DIMMATRIX 4      //dimensió de la matriu
#define SIZEMATRIX DIMMATRIX*DIMMATRIX //=16


/**
 * Definición de variables globales.
 **/
extern int developer;	 //Variable declarada en ensamblador que indica el nombre del programador


/**
 * Definición de las funciones en C.
 **/
void clearscreen_C();
void gotoxyP2_C(int, int);
void printchP2_C(char);
char getchP2_C();

char printMenuP2_C();
void printBoardP2_C();

void  showNumberP2_C(int, int, long);
void  updateBoardP2_C(int[DIMMATRIX][DIMMATRIX], long);
void  copyMatrixP2_C(int[DIMMATRIX][DIMMATRIX], int[DIMMATRIX][DIMMATRIX]);
void  rotateMatrixLRP2_C(int[DIMMATRIX][DIMMATRIX], char);
int   shiftNumbersLP2_C(int[DIMMATRIX][DIMMATRIX]);
int   addPairsLP2_C(int[DIMMATRIX][DIMMATRIX]);

void  insertTileP2_C(int m[DIMMATRIX][DIMMATRIX]);
short checkEndP2_C(int[DIMMATRIX][DIMMATRIX],int[DIMMATRIX][DIMMATRIX],short);
short printMessageP2_C(short);
void  playP2_C(int[DIMMATRIX][DIMMATRIX], int[DIMMATRIX][DIMMATRIX], int[DIMMATRIX][DIMMATRIX]);


/**
 * Definició de les subrutines d'assemblador que es criden des de C.
 */
extern void  showNumberP2(int, int, long);
extern void  updateBoardP2(int[DIMMATRIX][DIMMATRIX], long);
extern void  copyMatrixP2(int[DIMMATRIX][DIMMATRIX], int[DIMMATRIX][DIMMATRIX]);
extern void  rotateMatrixLRP2(int[DIMMATRIX][DIMMATRIX], char);
extern int   shiftNumbersLP2(int[DIMMATRIX][DIMMATRIX]);
extern int   addPairsLP2(int[DIMMATRIX][DIMMATRIX]);
extern short checkEndP2(int[DIMMATRIX][DIMMATRIX],int[DIMMATRIX][DIMMATRIX],short);
extern void  playP2(int[DIMMATRIX][DIMMATRIX], int[DIMMATRIX][DIMMATRIX], int[DIMMATRIX][DIMMATRIX]);


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 *   
 * Parámetros de salida: 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 **/
void clearScreen_C(){
   
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en una fila y una columna de la pantalla
 * en función de la fila (rowScreen) y de la columna (colScreen)
 * recibidos como parámetro.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * (rowScreen): rdi(edi): Fila.
 * (colScreen): rsi(esi): Columna.
 * 
 * Parámetros de salida: 
 * Ninguno
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'gotoxyP2' 
 * para poder llamar a esta función guardando el estado de los registros 
 * del procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 **/
void gotoxyP2_C(int rowScreen, int colScreen){
   
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter (c) en pantalla, recibido como parámetro,
 * en la posición donde está el cursor.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * (c): rdi(dil): Carácter que queremos mostrar.
 * 
 * Parámetros de salida: 
 * Ninguno
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'printchP2' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 **/
void printchP2_C(char c){
   
   printf("%c",c);
   
}


/**
 * Leer una tecla y devolver el carácter asociado 
 * sin mostrarlo en pantalla. 
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida: 
 * (c): rax(al): Carácter leído de teclado.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'getchP2' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 **/
char getchP2_C(){

   int c;   

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándar (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

   /* ~ICANON para tratar la entrada de teclado carácter a carácter no como línea completa acabada con /n
      ~ECHO para que no muestre el carácter leído*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente.*/
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   c=getchar();                 
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

   /*Devolver el carácter leído*/
   return (char)c;
   
}

/**
 * Mostrar en pantalla el menú del juego y pedir una opción.
 * Solo acepta una de las opciones correctas del menú ('0'-'9') o ESC.
 * 
 * Variables globales utilizadas:   
 * (developer):((char;)&developer): variable definida en el código ensamblador.
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida: 
 * (charac): rax(al): Opción elegida del menú, leída del teclado.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
char printMenuP2_C(){

 	clearScreen_C();
    gotoxyP2_C(1,1);
    printf("                                    \n");
    printf("           Developed by:            \n");
	printf("        ( %s )   \n",(char *)&developer);
    printf(" __________________________________ \n");
    printf("|                                  |\n");
    printf("|         MAIN MENU 2.0            |\n");
    printf("|__________________________________|\n");
    printf("|                                  |\n");
    printf("|         1. ShowNumber            |\n");
    printf("|         2. UpdateBoard           |\n");
    printf("|         3. CopyMatrix            |\n");
    printf("|         4. RotateMatrix          |\n");
    printf("|         5. ShiftNumbers          |\n");
    printf("|         6. AddPairs              |\n");
    printf("|         7. CheckEnd              |\n");
    printf("|         8. Play Game             |\n");
    printf("|         9. Play Game C           |\n");
    printf("|         0. Exit                  |\n");
    printf("|__________________________________|\n");
    printf("|                                  |\n");
    printf("|            OPTION:               |\n");
    printf("|__________________________________|\n"); 

    char charac =' ';
    while (charac < '0' || charac > '9') {
      gotoxyP2_C(21,22);
	  charac = getchP2_C();
	  printchP2_C(charac);
	}
	return charac;
   
}


/**
 * Mostrar el tablero de juego en la pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:	
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida: 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void printBoardP2_C(){

   gotoxyP2_C(1,1);                                                  //rowScreen
   printf(" _________________________________________________  \n"); //01
   printf("|                                                  |\n"); //02
   printf("|                  2048 PUZZLE  v2.0               |\n"); //03
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
   printf("| (ESC)Exit (u)Undo (i)Up (j)Left (k)Down (l)Right |\n"); //21
   printf("|__________________________________________________|\n"); //22
   
}


/**
 * Convierte el número de la variable (num) de tipo long de 6 dígitos 
 * (0 <= num <= 999999), recibido como parámetro, 
 * a caracteres ASCII que representan su valor. 
 * Si (num) es menor que 0 cambiamos el valor a 0, 
 * si es mayor que 999999 cambiaremos el valor a 999999. 
 * Se debe dividir(/) el valor entre 10, de forma iterativa, 
 * hasta obtener el 6 dígitos. 
 * En cada iteración, el residuo de la división (%) que es un valor 
 * entre (0-9) indica el valor del dígito a convertir 
 * a ASCII ('0' - '9') sumando '0' (48 decimal) para poder mostrarlo. 
 * Cuando el cociente sea 0 mostraremos espacios en la parte no significativa. 
 * Por ejemplo, si (num)=103 mostraremos "   103" y no "000103". 
 * Se deben mostrar los dígitos (carácter ASCII) desde la posición 
 * indicada por las variables (rowScreen) y (colScreen), recibidas como 
 * parámetro, posición de las unidades, hacia la izquierda. 
 * El primer dígito que obtenemos son las unidades, después las decenas, 
 * ..., para mostrar el valor se debe desplazar el cursor una posición 
 * a la izquierda en cada iteración. 
 * Para posicionar el cursor se llama a la función gotoxyP2_C y para 
 * mostrar los caracteres en la función printchP2_C. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (rowScreen) :rdi(edi): Fila de la pantalla donde posicionamos el cursor. 
 * (colScreen) :rsi(esi): Columna de la pantalla donde posicionamos el cursor. 
 * (num)       :rdx(rdx): Número que queremos mostrar. 
 * 
 * Parámetros de salida: 
 * Ninguno 
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'showNumberP2', 
 * el paso de parámetros es equivalente.
 */
 void showNumberP2_C(int rowScreen, int colScreen, long num) {
	
	char charac;
	int  i;

    if (num < 0) num = 0;
    else if (num > 999999) num = 999999;
	for (i=0;i<6;i++){
	  charac = ' ';
	  if (num > 0) {
		charac = (char)(num%10);   //residuo
		num = num/10;              //cociente
		charac = charac + '0';
	  } 
	  gotoxyP2_C(rowScreen, colScreen);
	  printchP2_C(charac);
	  colScreen--;
	}
		
}


/**
 * Actualizar el contenido del Tablero de Juego con los datos de la 
 * matriz (m) de 4x4 de tipo int y los puntos del marcador 
 * (score), recibido como parámetro, que se han hecho. 
 * Se debe recorrer toda la matriz (m), y para cada elemento de la matriz 
 * posicionar el cursor en la pantalla y mostrar el número 
 * de aquella posición de la matriz. 
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba abajo. 
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0]) 
 * a 60 (posición [3][3]) con incrementos de 4 porque los datos son de 
 * tipo int(DWORD) 4 bytes. 
 * Luego, mostrar el marcador (score) en la parte inferior del tablero, 
 * fila 18, columna 35 llamando la función showNumberP2_C. 
 * Finalmente posicionar el cursor en la fila 18, columna 36 llamando la 
 * función gotoxyP2_C. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (m)     :rdi(edi) : Matriz donde guardamos los números del juego. 
 * (score) :rsi(esi): Puntos acumulados en el marcador. 
 * 
 * Parámetros de salida: 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'updateBoardP2', 
 * el paso de parámetros es equivalente.
 */
void updateBoardP2_C(int m[DIMMATRIX][DIMMATRIX], long score){

   int i,j;
   int rowScreen, colScreen;
   
   rowScreen = 10;
   for (i=0;i<DIMMATRIX;i++){
	  colScreen = 17;
      for (j=0;j<DIMMATRIX;j++){
         showNumberP2_C(rowScreen,colScreen, (long)m[i][j]);
         colScreen = colScreen + 9;
      }
      rowScreen = rowScreen + 2;
   }
   showNumberP2_C(18, 35, score);   
   gotoxyP2_C(18,36);
   
}


/**
 * Copiar la matriz origen (mOrigin) -segundo parámetro- sobre la matriz 
 * destino (mDest) -primer parámetro-. 
 * La matriz origen (mOrigin) no debe modificarse,
 * los cambios se debe hacer sobre la matriz destino (mDest). 
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba abajo. 
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0]) 
 * a 60 (posición [3][3]) con incrementos de 4 porque los datos son de 
 * tipo int(DWORD) 4 bytes. 
 * Esto permitirá copiar dos matrices después de una rotación 
 * y gestionar la opción '(u)Undo' del juego. 
 * No se muestra la matriz. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (mDest)   :rdi(rdi): Dirección de la matriz donde guardamos los números del juego que queremos sobreescribir. 
 * (mOrigin) :rsi(rsi): Dirección de la matriz donde guardamos los números del juego que queremos copiar. 
 * 
 * Parámetros de salida: 
 * Ninguno 
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'copyMatrixP2', 
 * el paso parámetros es equivalente.
 */
void copyMatrixP2_C(int mDest[DIMMATRIX][DIMMATRIX], int mOrig[DIMMATRIX][DIMMATRIX]) {
	
	int i,j;
	
	for (i=0; i<DIMMATRIX; i++) {
		for (j=0; j<DIMMATRIX; j++) {
			mDest[i][j]=mOrig[i][j];
		}
	}

}


/**
 * Rotar a la derecha o a la izquierda la matriz (mToRotate) en la dirección 
 * indicada por la variable (dir: 'L' izquierda, 'R' derecha) sobre la 
 * matriz (mRotated), recibidas como parámetro. 
 * Si se rota a la izquierda (dir='L') 
 * la primera fila pasa a ser la primera columna,
 * la segunda fila pasa a ser la segona columna,
 * la tercera fila pasa a ser la tercera columna y
 * la cuarta fila pasa a ser la cuarta columna.
 * Si se rota a la derecha (dir='R') 
 * la primera columna pasa a ser la cuarta fila, 
 * la segunda columna pasa a ser la tercera fila, 
 * la tercera columna pasa a ser la segunda fila y 
 * la cuarta columna pasa a ser la primera fila. 
 * En el enunciado se explica en más detalle cómo hacer la rotación. 
 * NOTA: NO es lo mismo que hacer la matriz transpuesta. 
 * La matriz (mToRotate) no debe modificarse, 
 * los cambios deben realizarse en la matriz (mRotated). 
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0]) 
 * a 60 (posición [3][3]) con incrementos de 4 porque los datos son de 
 * tipo int(DWORD) 4 bytes. 
 * Para acceder a una posición concreta de la matriz desde ensamblador 
 * hay que tener en cuenta que el índice es:(index=(fila*DIMMATRIX+columna)*4), 
 * multiplicamos por 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * Una vez realizada la rotación, copiar la matriz (mRotated) en la matriz 
 * (mToRotate) recibidas como parámetro llamando la función copyMatrixP2_C. 
 * No se debe mostrar la matriz. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (mToRotate):rdi(rdi): Dirección de la matriz donde guardamos los números del juego que queremos rotar. 
 * (dir)      :rsi(sil): Dirección de la rotación: 'L' izquierda, 'R' derecha. 
 * 
 * Parámetros de salida: 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'rotateMatrixRP2', 
 * el paso de parámetros es equivalente.
 */
void rotateMatrixLRP2_C(int mToRotate[DIMMATRIX][DIMMATRIX], char dir) {
	
   int mRotated[DIMMATRIX][DIMMATRIX];
   int i,j;

   for (i=0; i<DIMMATRIX; i++) {
      for (j=0; j<DIMMATRIX; j++) {
         if(dir=='L') mRotated[DIMMATRIX-1-j][i] = mToRotate[i][j];
         if(dir=='R') mRotated[j][DIMMATRIX-1-i] = mToRotate[i][j];
      }
   }
	
   copyMatrixP2_C(mToRotate, mRotated);
	
}


/**
 * Desplaza los números de cada fila de la matriz (mShift) a la izquierda 
 * recibida como parámetro, manteniendo el orden de los números y poniendo 
 * los ceros a la derecha si se desplaza. 
 * Recorrer la matriz por filas de izquierda a derecha y de arriba abajo. 
 * Para recorrer la matriz en ensamblador, el índice va de la 
 * posición 0 (posición [0][0]) a la 60 (posición [3][3]) con incrementos 
 * de 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * Para acceder a una posición concreta de la matriz desde ensamblador 
 * hay que tener en cuenta que el índice es:(index=(fila*DIMATRIX+columna)*4), 
 * multiplicamos por 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * En cada fila, si encuentra un 0, mira si hay un número diferente de cero, 
 * en la misma fila para ponerlo en esa posición. 
 * Si se desplaza un número (NO LOS CEROS), se deben contar los 
 * desplazamiento incrementando la variable (shifts). 
 * Si una fila de la matriz es: [0,2,0,4] y (shifts = 0), quedará 
 * [2,4,0,0] y (shifts = 2). 
 * Devolvemos el valor de la variable (shifts) para indicar el número 
 * de desplazamientos que se han hecho. 
 * Los cambios deben realizarse sobre la misma matriz. 
 * No se debe mostrar la matriz. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (mShift) :rdi(rdi): Dirección de la matriz donde guardamos los números del juego que queremos desplazar. 
 * 
 * Parámetros de salida: 
 * (shifts) :rax(eax): Desplazamientos que se han hecho. 
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'shiftNumbersLP2', 
 * el paso de parámetros es equivalente.
 */
int shiftNumbersLP2_C(int mShift[DIMMATRIX][DIMMATRIX]) {
	
   int i,j,k;
   int shifts=0;
	
   for (i=0; i<DIMMATRIX; i++) {
		for (j=0; j<DIMMATRIX-1; j++) {
			if (mShift[i][j] == 0) {
				k = j+1;
				while (k<DIMMATRIX && mShift[i][k]==0) k++;
				if (k<DIMMATRIX) {
					mShift[i][j]=mShift[i][k];
					mShift[i][k]= 0; 
					shifts++;
				}
			}		
		}
	}

   return shifts;
	
}
	

/**
 * Emparejar números iguales de la matriz (mPairs) hacia la izquierda, 
 * recibida como parámetro, y acumular los puntos en el marcador sumando los 
 * puntos (p) de las parejas que se hayan hecho. 
 * Recorrer la matriz por filas de izquierda a derecha y de arriba a abajo.
 * Cuando se encuentre una pareja, dos casillas consecutivas de la misma 
 * fila con el mismo número, juntamos la pareja poniendo la suma de la 
 * pareja en la casilla de la izquierda, un 0 en la casilla de la derecha y
 * acumularemos esta suma en la variable (p) (puntos ganados). 
 * Si una fila de la matriz es: [4,4,2,2] quedará [8,0,4,0] y 
 * p = p + (8+4). 
 * Devolveremos los puntos (p) obtenidos de hacer las parejas. 
 * Para recorrer la matriz en ensamblador, el índice va de la 
 * posición 0 (posición [0][0]) a la 60 (posición [3][3]) con incrementos 
 * de 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * Para acceder a una posición concreta de la matriz desde ensamblador 
 * hay que tener en cuenta que el índice es:(index=(fila*DIMMATRIX+columna)*4), 
 * multiplicamos por 4 porque los datos son de tipo int(DWORD) 4 bytes. 
 * Los cambios deben realizarse sobre la misma matriz. 
 * No se debe mostrar la matriz. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (mPairs) :rdi(edi): Dirección de la matriz donde guardamos los números del juego que queremos hacer parejas. 
 * 
 * Parámetros de salida: 
 * (p) : rax(eax): Puntos de las parejas que se hayan hecho. 
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'addPairsLP2', 
 * el paso de parámetros es equivalente.
 */
int addPairsLP2_C(int mPairs[DIMMATRIX][DIMMATRIX]) {
	
   int i,j;
   int p=0;
	
   for (i=0; i<DIMMATRIX; i++) {
     for (j=0; j<DIMMATRIX-1; j++) {
       if ((mPairs[i][j]!=0) && (mPairs[i][j]==mPairs[i][j+1])) {
         mPairs[i][j]  = mPairs[i][j]*2;
         mPairs[i][j+1]= 0;
         p = p + mPairs[i][j];
         j++;
       }		
     }
   }
   
   return p;
	
}


/**
 * Verificar si se ha llegado a 2048 o si no se puede realizar algún movimiento. 
 * Si existe el número 2048 en la matriz (m), cambiar 
 * el estado a 4 (state=4) para indicar que se ha ganado (WIN!). 
 * Si no hemos ganado, mirar si se puede hacer algún movimiento, 
 * Si no se puede hacer ningún movimiento cambiar el estado a 5 (state=5) para 
 * indicar que se ha perdido (GAME OVER!!!). 
 * Recorrer la matriz (m) por filas de derecha a izquierda y de abajo arriba 
 * contando las casillas vacías y mirando si existe el número 2048. 
 * Si existe el número 2048 poner (status=4) y terminar. 
 * Si no existe el número 2048 y no hay casillas vacías mirar si se puede 
 * hacer algún emparejamiento en horizontal o en vertical. Para ello, es necesario 
 * copiar la matriz (m) sobre la matriz (mAux) llamando la función 
 * copyMatrixP2_C, hacer parejas en la matriz (mAux) para mirar si se 
 * pueden hacer parejas en horizontal llamando la función addPairsLP2_C y 
 * guardar los puntos obtenidos, rotar la matriz (mAux) llamando la 
 * función rotateMatrixLRP2_C y volver a hacer parejas en la matriz (mAux) 
 * para mirar si se pueden hacer parejas en vertical llamando la función 
 * addPairsLP2_C y acumular los puntos obtenidos con los puntos 
 * obtenidos antes, si los puntos acumulados son 0, significa que no se 
 * pueden hacer parejas y debe ponerse el estado del juego a 5 (status=5). 
 * No se puede modificar ni la matriz (m), ni la matriz (mUndo). 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (m)     :rdi(rdi): Dirección de la matriz donde guardamos los números del juego. 
 * (mAux)  :rsi(rsi): Dirección de la matriz donde copiamos la matriz (m) para comprobarla. 
 * (state) :rdx(di) : Estado del juego. 
 * 
 * Parámetros de salida: 
 * (state) :rax(ax) : Estado del juego. 
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'checkEndP2', 
 * el paso de parámetros es equivalente.
 */
short checkEndP2_C(int m[DIMMATRIX][DIMMATRIX], int mAux[DIMMATRIX][DIMMATRIX], short state) {
	
	int i,j;
	int zeros=0;
	int pairs=0;
		
	i=DIMMATRIX;
	do {
		i--;
		j=DIMMATRIX;
		do {
			j--;
			if (m[i][j] == 0 ) zeros++;
			if (m[i][j] == 2048) state=4;
		} while ((j>0) && (m[i][j]!=2048));
		
	} while ((i>0) && (m[i][j]!=2048));

	if ((state!= 4) && (zeros == 0)) {
		copyMatrixP2_C(mAux,m);
		pairs = addPairsLP2_C(mAux);
		rotateMatrixLRP2_C(mAux,'R');
		pairs = pairs + addPairsLP2_C(mAux);
		if (pairs==0) state = 5;	
	}
	
	return state;
} 


/**
 * Generar nueva ficha de forma aleatoria. 
 * Si hay al menos una casilla vacía en la matriz (m) genera una 
 * fila y una columna de forma aleatoria hasta que es una de las casillas 
 * vacías. 
 * A continuación generar un número aleatorio para decidir si la nueva ficha 
 * debe ser un 2 (90% de los casos) o un 4 (10% de los casos). 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (m) :rdi(edi): Dirección de la matriz donde guardamos los números del juego. 
 * 
 * Parámetros de salida: 
 * Ninguno
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'insertTileP2' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 */
void insertTileP2_C(int m[DIMMATRIX][DIMMATRIX]) {
	
	int i,j,k,l,r;
	
	i=DIMMATRIX; // Miramos si hay una casilla vacia.
	do {
		i--;
		j=DIMMATRIX;
		do {
			j--;	
		} while ((j>0) && (m[i][j]!=0));
	} while ((i>0) && (m[i][j]!=0));
	
	////Insertamos el 2 o el 4 si hay como a mínimo una casilla vacía.
	if (m[i][j]==0) { 
		do { // Genera filas y columnas aleatoriamente hasta encontrar
			 // una posición vacía.
			k = rand() % 4; l = rand() % 4; }
		while( m[k][l] != 0 );
		
		//Decidimos si ponemos un 2 o un 4.
		r = rand() % 100;
		if( r < 90 ) {
			m[k][l] = 2;
		} else {
			m[k][l] = 4;
		}
	}

}


/**
 * Mostrar un mensaje debajo del tablero según el valor de la variable 
 * (state). 
 * state: 0: Hemos pulsado la tecla 'ESC' para salir del juego. 
 *        1: Continuamos jugando. 
 *        2: Continuamos jugando pero ha habido cambios en la matriz. 
 *        3: Deshacer último movimiento 
 *        4: Ganado, ya tenemos la ficha 2048. 
 *        5: Perdido, no se puede hacer ningún movimiento. 
 * Se espera que se pulse una tecla para continuar. 
 * 
 * Si se ha perdido (state=5) se da la opción de hacer un último 'Undo' 
 * pulsando la tecla 'u' para recuperar el estado anterior y probar de 
 * continuar jugando (state=3). Si se pulsa otra tecla se sale del juego. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (state) :rdi(di): Estado del juego. 
 * 
 * Parámetros de salida: 
 * Ninguno
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'printMessageP2' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 */
short printMessageP2_C(short state) {

   switch(state){
     case 0:
       gotoxyP2_C(23,12);
       printf("<<<<<< EXIT: (ESC) Pressed >>>>>>");
       getchP2_C();
       break;
     case 4:
       gotoxyP2_C(23,12);
       printf("++++++ 2048!!!  YOU  W I N ++++++");
       getchP2_C();
       break;
     case 5:
       gotoxyP2_C(23,12);
       printf("---- G A M E   O V E R ! ! ! ----");
       gotoxyP2_C(24,12);
       printf("---- (u)Undo  (Any key) EXIT ----");
       char c;
        c = getchP2_C();
		if (c == 'u') {
          gotoxyP2_C(23,12);
          printf("                                  ");
          gotoxyP2_C(24,12);
          printf("                                  ");
          state=3;
       }
       break;
   }
   return state;
}
 

/**
 * Juego de 2048 
 * Función principal del juego 
 * Permite jugar al juego del 2048 llamando a todas las funcionalidades. 
 * 
 * Pseudo código: 
 * Inicializar estado del juego, (state=1). 
 * Mostrar el tablero de juego (llamar la función printBoardP2_C). 
 * Actualiza el contenido del Tablero de Juego y los puntos que se han hecho 
 * (llamar la función updateBoardP2_C). 
 * Mientras (state==1) hacer: 
 *   Copiar la matriz (m) sobre la matriz (mAux) llamando la función 
 *   copyMatrixP2_C y copiar los puntos (score) sobre (scoreAux). 
 *   Leer una tecla (c) llamando la función getchP2_C. 
 *   Según la tecla leída llamaremos a las funciones correspondientes. 
 *     ['i' (arriba),'j'(izquierda),'k' (abajo) o 'l'(derecha)] 
 *     Rotar la matriz (m) llamando la función rotateMatrixLRP2_C, para 
 *     poder hacer los desplazamientos de los números a la izquierda llamando 
 *     la función shiftNumbersLP2_C y hacer las parejas llamando 
 *     la función addPairsLP2_C que también actualiza los puntos (p) que 
 *     se deben sumar al marcador (score), volver a desplazar 
 *     los números a la izquierda llamando la función shiftNumbersLP2_C 
 *     con las parejas hechas. 
 *     Luego, volver a rotar llamando la función rotateMatrixLRP1_C 
 *     para dejar la matriz en la posición inicial. 
 *     Para la tecla 'j' (izquierda) no es necesario hacer rotaciones, 
 *    para el resto se deben hacer rotaciones. 
 *     'u' Poner (state = 3) para recuperar el estado anterior. 
 *     '<ESC>' (ASCII 27) poner (state ='0) para salir del juego. 
 *     Si no es ninguna de estas teclas no hacer nada. 
 *     Actualizar el marcador (score) con el punto de las parejas hechas 
 *     (score = score + p) 
 *   Si hemos desplazado algún número o hemos hecho alguna pareja 
 *   (((s1>0) || (p>0) || (s2>0)))), copiar el estado del juego que hemos 
 *   guardado en (mAux y scoreAux) en (mUndo y scoreUndo) para poder hacer 
 *   el Undo (recuperar estado anterior) copiando (mAux) sobre (mUndo) 
 *   llamando a la función copyMatrixP2_C y haciendo (scoreUndo = scoreAux). 
 *   Generar una nueva ficha llamando la función insertTileP2_C y poner 
 *   la variable state a 1 (state=1). 
 *   Actualiza el contenido del Tablero de Juego y los puntos que se han hecho 
 *   llamando a la función updateBoardP2_C. 
 *   Verificar si se ha llegado a 2048 o si se pueden realizar movimientos 
 *   llamando la función CheckEndP2_C. 
 *   Mostrar un mensaje debajo del tablero según el valor de la variable 
 *   (state) llamando a la función printMessageP2_C. 
 *   Si debemos recuperar el estado anterior (state=3), copiar el estado 
 *   anterior del juego que tenemos en (mUndu y scoreUndu) sobre (m y score) 
 *   llamando a la función copyMatrixP2_C y haciendo (score = scoreUndo), 
 *   poner la variable state a 1 (state=1) y actualiza el contenido 
 *   del Tablero de Juego y los puntos que se han hecho llamando la función 
 *   updateBoardP2_C. 
 * Fin mientras. 
 * Salir: 
 * Se terminado el juego. 
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada: 
 * (m)     :rdi(rdi): Dirección de la matriz donde guardamos los números del juego. 
 * (mAux)  :rsi(rsi): Dirección de la matriz donde copiamos la matriz (m) para comprobarla. 
 * (mUndu) :rdx(rdx): Dirección de la matriz donde guardamos el último movimiento para poder volver atrás. 
 * 
 * Parámetros de salida: 
 * Ninguna
 */
void playP2_C(int m[DIMMATRIX][DIMMATRIX],int mAux[DIMMATRIX][DIMMATRIX],int mUndo[DIMMATRIX][DIMMATRIX]){
   short state = 1;// Estado del juego.
                   // 0: Salir, hemos pulsado la tecla 'ESC' para salir.
                   // 1: Continuemos jugando.
                   // 2: Continuamos jugando pero se han hecho cambios en la matriz.
                   // 3: Deshacer último movimiento.
	               // 4: Ganado, se ha obtenido la casilla 2048.
                   // 5: Perdido, no se pueden hacer movimientos.
			    	     
   long score     = 290500;
   long scoreAux  = 0;
   long scoreUndo = 1;
   
   int s1, s2, p;
   char c;   			   
   
   printBoardP2_C();
   updateBoardP2_C(m, score);
          
   while (state == 1) {
	 copyMatrixP2_C(mAux,m);  
	 scoreAux = score;

     c = getchP2_C();
     
     s1 = 0;
     p  = 0;
     s2 = 0;
     switch(c){
       case 'i': //i:(105) up
         rotateMatrixLRP2_C(m,'L');
		 
         s1 = shiftNumbersLP2_C(m);
         p  = addPairsLP2_C(m);
         s2 = shiftNumbersLP2_C(m);
                  
         rotateMatrixLRP2_C(m,'R');
		 
       break;
       case 'j': //j:(106) left
         s1 = shiftNumbersLP2_C(m);
         p  = addPairsLP2_C(m);
         s2 = shiftNumbersLP2_C(m);
         
       break;
       case 'k': //k:(107) down
         rotateMatrixLRP2_C(m,'R');
             
         s1 = shiftNumbersLP2_C(m);
         p  = addPairsLP2_C(m);
         s2 = shiftNumbersLP2_C(m);
         
		 rotateMatrixLRP2_C(m,'L');
       break;
       case 'l': //l:(108) right
         rotateMatrixLRP2_C(m,'R');
         rotateMatrixLRP2_C(m,'R');
       
         s1 = shiftNumbersLP2_C(m);
         p  = addPairsLP2_C(m);
         s2 = shiftNumbersLP2_C(m);
         
         rotateMatrixLRP2_C(m,'L');
         rotateMatrixLRP2_C(m,'L');
       
       break;  
	   case 'u': //u:(117) Undu
         state = 3;
	   break;
	   case 27: //ESC:(27) exit
		 state = 0;
	   break;
	 }
	 
	 score = score + (long)p;
	 
     if ( (s1>0) || (p>0) || (s2>0) ) { //(state == 2) {	  
       copyMatrixP2_C(mUndo, mAux);
       scoreUndo = scoreAux;
       insertTileP2_C(m);	  
       state = 1;
	 }
	 
     updateBoardP2_C(m, score);
	 state = checkEndP2_C(m, mAux, state);
	 state = printMessageP2_C(state);
	 
	 if (state == 3) {	 
	   copyMatrixP2_C(m, mUndo);
       score = scoreUndo;
       state = 1;
       updateBoardP2_C(m, score);
	 }     
	 	
  }

}


/**
 * Programa Principal
 * 
 * ATENCIÓN: En cada opción se llama a una subrutina de ensamblador.
 * Debajo hay comentada la función en C equivalente que os damos hecha 
 * por si queréis ver como funciona.
 */
int main(){   
   // Matriz 4x4 donde guardaremos los números del juego
   // Acceso a las matrices en C: utilizamos fila (0..[DIMMATRIX-1]) y
   // columna(0..[DIMMATRIX-1]) (m[fila][columna]).
   // Acceso a las matrices en ensamblador: Se accede a ella como si fuera un vector
   // donde indexMat (0..[DIMMATRIX*DIMMATRIX-1]).
   // indexMat=((fila*DIMMATRIX)+(columna))*4 (4 porque la matriz es de tipo int).
   // DWORD[M+indexMat] (DWORD porque es de tipo int)
   // (indexMat debe ser un registro de tipo long/QWORD:RAX,RBX,..,RSI,RDI,..,R15).
   int m[DIMMATRIX][DIMMATRIX]        = { {    8,    8,    32,    32},
                                          {    4,   32,   128,    64},
                                          {    0,    0,   256,   128},
                                          {    0,    4,   512,  1024} };

   int mAux[DIMMATRIX][DIMMATRIX]     = { {    0,    0,     0,     0},
                                          {    0,    0,     0,     0},
                                          {    0,    0,     0,     0},
                                          {    0,    0,     0,     0} };
                                    
   int mUndo[DIMMATRIX][DIMMATRIX]    = { {    0,    1,     3,     7},
                                          {   15,   31,    63,   127},
                                          {  255,  511,  1023,  2047},
                                          { 4095, 8191, 16383, 32767},};

   long score     = 123456;
   long scoreAux  = 0;
   long scoreUndo = 1;
   short state = 1;
   
   char op=' ';
   char c;
   
   while (op!='0') {
	  op = printMenuP2_C();
      
      switch(op){
         case '1':// Mostrar puntos
            clearScreen_C();  
            printBoardP2_C();   
            gotoxyP2_C(25, 20);
            printf(" Press any key ");
            //=======================================================
            showNumberP2(18, 35, 1000000);       
            ///showNumberP2_C(18, 35 , 1000000);   
            //=======================================================
            getchP2_C();
         break;
         case '2': //Actualizar el contenido del tablero.
            clearScreen_C();  
            printBoardP2_C(); 
            //=======================================================
            updateBoardP2(m, score);
            ///updateBoardP2_C(m, score); 
            //=======================================================
            gotoxyP2_C(25, 20);
            printf("Press any key ");
            getchP2_C();
         break;
         case '3': //Copiar matriz.
            clearScreen_C();  
            printBoardP2_C(); 
            int scoreUndu = 500;
            //=======================================================
            copyMatrixP2(m, mUndo);
            ///copyMatrixP2_C(m, mUndo);
            //=======================================================
            updateBoardP2_C(m, scoreUndu);
            gotoxyP2_C(25, 20);
			printf("Press any key ");
	        getchP2_C();
         break;
         case '4': //Rotar matriz
            clearScreen_C();  
            printBoardP2_C(); 
            //===================================================
            rotateMatrixLRP2(m,'L');
			///rotateMatrixLRP2_C(m, 'L');
			//===================================================
            updateBoardP2_C(m, score);
            gotoxyP2_C(25, 20);
			printf("Press any key ");
	        getchP2_C();
          break;
          case '5': //Desplazar números
            clearScreen_C();  
            printBoardP2_C(); 
            //===================================================
            shiftNumbersLP2(m);
			///shiftNumbersLP2_C(m);
			//===================================================
            updateBoardP2_C(m, score);
            gotoxyP2_C(25, 20);
			printf("Press any key ");
	        getchP2_C();
          break;
          case '6': //Hacer parejas y sumar puntos
            clearScreen_C();  
            printBoardP2_C();   
            score = 1000;
            //===================================================
			score = score + addPairsLP2(m);
			///score = score + addPairsLP2_C(m);
			//===================================================
			updateBoardP2_C(m, score);
			gotoxyP2_C(25, 20);
			printf("Press any key ");
	        getchP2_C();
         break;
         case '7': //Verificar si hay el 2048 o si se puede hacer algún movimiento.
            clearScreen_C();        
            printBoardP2_C();       
            updateBoardP2_C(m, score); 
            //===================================================
			state = checkEndP2(m, mAux, state);
			///state = checkEndP2_C(m, mAux, state);
			//===================================================
			state = printMessageP2_C(state);
			if ((state!='4') && (state!='5')) {
			   gotoxyP2_C(25, 20);
			   printf("Press any key ");
	           getchP2_C();
	        }
         break;
         case '8': //Juego completo Ensamblador
           clearScreen_C(); 
           //=======================================================
           playP2(m, mAux, mUndo);
           //=======================================================
         break;
         case '9': //Juego completo en C 
           clearScreen_C(); 
           //=======================================================
           playP2_C(m, mAux, mUndo);
           //=======================================================
         break;
      }
   }
   printf("\n");
   
   return 0;
   
}
