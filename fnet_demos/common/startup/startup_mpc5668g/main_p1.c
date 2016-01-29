/* Simple application that does absolutely nothing useful.  */
/*                                                          */
/* Note that this program does NOT initialize the interrupt */
/* controller or provide any service routines.              */
/* According to the preliminary reference manual, processor */
/* 0 (Z1) receives all interrupt requests following a reset.*/
/*                                                          */
/* Please see the projects in (CodeWarrior_Examples) for    */
/* more complete examples.                                  */


int main_p1(void) {
  volatile int i = 0;
 
  
  /* Loop forever */
  for (;;) {
    i++;
  }
}



