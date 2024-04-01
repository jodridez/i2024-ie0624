//Includes



//Declaracion de funciones
/*
delay: Funcion facilitada por el profesor en el ejemplo 
de hola PIC, genera un retardo.
*/
void delay (unsigned int tiempo);

/*
rand: Genera numeros aleatorios mediante software con contadores.
La funcion recibe la señal de un PIN de entrada, mientras este en
alto cuenta hasta alcanzar el limite randMax.
Genera numeros aleatorios en el rango de 0 a randMax.
*/
int rand(int randMax, int PIN);

/*
rollDice: Utiliza la funcion rand para generar numeros aleatorios
del 1 al 6, simulando el resultado de una tirada de dado.
Recibe el PIN a leer.
*/
int rollDice(int PIN);


//Defincion de funciones
void delay (unsigned int tiempo){
  unsigned int
  i ;
  unsigned int j ;
  for( i =0; i <tiempo ; i ++)
  for( j =0; j <1275; j ++);
}

int rand(int randMax, int PIN){
   int ramdonNumber = 0;
   if (PIN == 1){
      while (PIN == 1){
         ramdonNumber++;
         if (ramdonNumber>randMax){
            ramdonNumber = 0;
         }
      }
   }
   return ramdonNumber;
}

int rollDice(int PIN){
   int resultado = rand(7,PIN) + 1;
   return resultado;
}




