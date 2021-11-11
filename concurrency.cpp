#include <iostream>
#include <cstdlib> //for srand and rand
#include <time.h> //for time used to generate a seed
#include <unistd.h>
//#include <sys/shm.h>  //for shmget

//*** From stackoverflow guide
#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */

#include <sys/wait.h> 

//*********NOTE At LINE ~30 a specific directory is used************************
void boardWriter(char board[][5]); //function to write board state
char victory(char board[][5]); //function to determine if a player won
void randomPlay(int& row, int& col, char board[][5], char player); //function to randomly select a location for play
int sem_create();  //a function to create a semaphore

int main()
{
  srand(time(NULL));      //generate a seed for the rand function

  char gameboard[5][5];                                   //create a blank gameboard
  std::fill(gameboard[0], gameboard[0] + 5 * 5, '-');     //fill the empty spots with "-"



  key_t shmkey;     //shared memory key

  //*********Directory Identified HERE************************
  shmkey = ftok ("/dev/null", 5);       //valid directory name and number
  //**************************************************************************
  int shmid;    //shared memory id
  shmid = shmget (shmkey, sizeof (int), 0644 |IPC_CREAT);
  if (shmid < 0) {
    perror("shmget\n");
    exit (1);
  }

  int * p = (int * ) shmat (shmid, NULL, 0);   // attach p to shared memory
  *p = 0;
  printf ("p=%d is allocated in shared memory. \n\n", *p);

  sem_t *sem;     //synch semaphore   //*shared
  //unsigned int value;   //semaphore value
  sem = sem_open ("producer", O_CREAT | O_EXCL, 0644, 1); //name of semaphore is "producer"; semaphore can be reached using this Name
  boardWriter(gameboard);     //producer creates a blank gameboard

  //pid_t playerX, playerO, referee;     //create ID's for three difference processes


/*
  playerX = fork ();           //create fork for one player
  if (playerX < 0) {         //check for error
    sem_unlink ("producer");
    sem_close(sem);
    std::cout <<"Fork Error \n";
    exit(0);
  }

  playerO = fork ();           //create fork for second player
  if (playerO < 0) {         //check for error
    sem_unlink ("producer");
    sem_close(sem);
    std::cout <<"Fork Error \n";
  }

  referee = fork ();           //create fork for referee
  if (playerX < 0) {         //check for error
    sem_unlink ("producer");
    sem_close(sem);
    std::cout <<"Fork Error \n";
  }
*/
pid_t pid;
int i{};
for (i; i < 3; i++) {
  pid = fork();
  if (pid < 0){
    sem_unlink ("producer");
    sem_close(sem);
    std::cout <<"Fork Error \n";
  }
}

//************************
//*******Producer

  if (pid !=0) {
    //wait for child to exit
    while (pid = waitpid (-1, NULL, 0)) {
        if (errno == ECHILD)
          break;
    }

    std::cout <<"\n All children have exited. \n";

    //shared memory detach
    shmdt (p);
    shmctl (shmid, IPC_RMID, 0);

    /* cleanup semaphores */
    sem_unlink ("pSem");
    sem_close(sem);
    /* unlink prevents the semaphore existing forever */
    /* if a crash occurs during the execution         */
    exit (0);
  }

  //********child
  else {
    sem_wait (sem);  //P operation
    printf ("  Child(%d) is in critical section.\n", i);
    sleep (1);
    sem_post (sem);   //v operation
    exit(0);
  }
  return EXIT_SUCCESS;
}
//**********TEST LOOP *************************
/*
  int x, y;
  for(int i{}; i<10; i++)
  {
    randomPlay(x,y, gameboard, 'X');
    randomPlay(x,y, gameboard, 'O');
    char winnerFound = victory(gameboard);
    if (winnerFound != '-')
    {  std::cout << "\nWe have a Winner!!!!!!\n"
      << "Player "<<winnerFound << " has won the game\n";
      break;
    }  else
      std::cout << "No winner yet\n";
  }
//**************</ TEST LOOP> ************
  return EXIT_SUCCESS;
}*/

void boardWriter(char board[][5])
{
  for(int rows{0}; rows < 5; rows++)
  {
    for (int i {0}; i<21; i++)
    {
      std::cout <<"_";
    }
    std::cout <<"\n";
    for (int cols{}; cols < 5; cols++)
    {
      std::cout<<"| " <<board[rows][cols] <<" ";
    }
    std::cout<<"|\n";
  }
  for (int i {0}; i<21; i++)
  {
    std::cout <<"_";
  }
  std::cout <<"\n";
}

char victory(char board[][5])
{
  char winner = '-';
  for(int row{}; row<5; row++)               //outter loop to move down rows
  {
    for(int cols{};cols<5; cols++)          //inner loop to move across columns
    {

      if(board[row][cols] != '-'&& board[row][cols] == board[row][cols+1])      //check for horizontal winner
        if(board[row][cols] != '-'&& board[row][cols] == board[row][cols+2])
          return winner = board[row][cols];

      if(board[row][cols] != '-'&& board[row][cols] == board[row+1][cols])        //check for vertical winner
        if(board[row][cols] != '-'&& board[row][cols] == board[row+2][cols])
          return winner = board[row][cols];

      if(board[row][cols] != '-'&& board[row][cols] == board[row+1][cols+1])    //check for Right diagonal winnerFound
        if(board[row][cols] != '-'&& board[row][cols] == board[row+2][cols+2])
          return winner = board[row][cols];

      if(board[row][cols] != '-'&& board[row][cols] == board[row+1][cols-1])    //check for Left diagonal winnerFound
        if(board[row][cols] != '-'&& board[row][cols] == board[row+2][cols-2])
          return winner = board[row][cols];
    }
  }
  return winner;
}

void randomPlay(int& row, int& col, char board[][5], char player)
{
    int random_max = 5;
    row = rand() % random_max +1;
    //std::cout << "Row in randomPlay" << row <<"\n";
    col = rand() % random_max +1;
    //std::cout << "Col in randomPlay" << col <<"\n";
    while(board[row][col] !='-')
    {
      row = rand() % random_max +1;
      col = rand() % random_max +1;
    }
    board[row][col] = player;
    boardWriter(board);
}
/*int sem_create()
{
  if((shm_id = shmget(IPC_PRIVATE, sizeof(int), SHM_R | SHM_W)) < 0) {
    perror(NULL);
    std::cout <<"Can't create shared memory segment. Goodbye\n";
    return EXIT_FAILURE;
  } else {

  }
  return  ;
}*/
