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
#include <sys/mman.h>

//*********NOTE At LINE ~30 a specific directory is used************************
void boardWriter(char board[][5]); //function to write board state
char victory(char board[][5]); //function to determine if a player won
//void randomPlay(int& row, int& col, char board[][5], char player); //function to randomly select a location for play
void randomPlay(char board[][5], char player);
int sem_create();  //a function to create a semaphore

int main()
{
  srand(time(NULL));      //generate a seed for the rand function



  //char * gameboard[5][5];
  //char gameboard[5][5];                                   //create a blank gameboard
  //std::fill(gameboard[0], gameboard[0] + 5 * 5, '-');     //fill the empty spots with "-"



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

  //char gameboard[5][5];
  //char* gameboard[5][5];
  //gameboard = (char*) shmat(shmid, NULL, 0);
  //gameboard = shmat(shmid, (void *)0, 0);
  //gameboard = shmat(shmid, NULL, 0);
  //gameboard[5][5];
  //std::fill(gameboard[0], gameboard[0] + 5 * 5, '-');


  int ARRAY_SIZE = (5)*(5)*sizeof(char);
  char (*gameboard)[5] = (char (*)[5]) mmap(NULL, ARRAY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  std::fill(gameboard[0], gameboard[0] + 5 * 5, '-');     //fill the empty spots with "-"

  int * p = (int * ) shmat (shmid, NULL, 0);   // attach p to shared memory
  *p = 0;
  printf ("p=%d is allocated in shared memory. \n\n", *p);

  //gameboard = shmat (shmid, NULL, 0);

  sem_t *sem;     //synch semaphore   //*shared
  //unsigned int value;   //semaphore value
  sem = sem_open ("producer", O_CREAT | O_EXCL, 0644, 1); //name of semaphore is "producer"; semaphore can be reached using this Name
  boardWriter(gameboard);     //producer creates a blank gameboard



//**********TEST LOOP *************************

/*
for(int i{}; i<25; i++)
  {
    randomPlay(gameboard, 'X');
    randomPlay(gameboard, 'O');
    char winnerFound = victory(gameboard);
    if (winnerFound == '-')
    {
      std::cout << "No winner yet\n";
    }  else if (winnerFound == 'Z') {
      std::cout << "The Game is a TIE\n";
      break;
    } else {
      std::cout << "\nWe have a Winner!!!!!!\n"
       << "Player "<<winnerFound << " has won the game\n";
       break;
    }
  }
//**************</ TEST LOOP> ************
  return EXIT_SUCCESS;
*/





pid_t pid;
int i{};

/*
for (i; i < 3; i++) {
  pid = fork();
  if (pid < 0){
    sem_unlink ("producer");
    sem_close(sem);
    std::cout <<"Fork Error \n";
  }
  else if (pid ==0)
    break;
  //std::cout<<"TEST  fork PID is: "<<pid <<"\n";
}*/

//************************
/*
if (playerX = fork ()<0) {           //create fork for one player                   //check for error
  sem_unlink ("producer");
  sem_close(sem);
  std::cout <<"Fork Error \n";
  exit(0);
} else if (playerX == 0) {
  sem_wait(sem);
  std::cout<<"Player X is playing\n";
  randomPlay(gameboard, "X");
}*/







//*******Producer
pid_t playerX, playerO, referee;     //create ID's for three difference processes
char winnerFound;

/*
playerX = fork ();           //create fork for one player
if (playerX < 0) {         //check for error
  sem_unlink ("producer");
  sem_close(sem);
  std::cout <<"Fork Error \n";
  exit(0);
}
std::cout <<"playerX is: " << playerX <<"\n";

playerO = fork ();           //create fork for second player
if (playerO < 0) {         //check for error
  sem_unlink ("producer");
  sem_close(sem);
  std::cout <<"Fork Error \n";
}
std::cout <<"playerO is: " << playerO <<"\n";
referee = fork ();           //create fork for referee
if (playerX < 0) {         //check for error
  sem_unlink ("producer");
  sem_close(sem);
  std::cout <<"Fork Error \n";
}
std::cout <<"referee is: " << referee <<"\n";

if (playerX = fork() == 0) {
    sem_wait(sem);
    randomPlay(gameboard, 'X');
    sem_post(sem);
}  else if (playerO = fork == 0) {
  sem_wait (sem);
  randomPlay(gameboard, 'O');
  sem_post (sem);
} else if (referee = fork() == 0){
  sem_wait (sem);
  winnerFound = victory(gameboard);
} else {
  while(wait(NULL)>0);
  std::cout <<"All children clear\n";
}*/













/*  char winnerFound;
  while (winnerFound != 'X' || winnerFound != 'Z' || winnerFound != 'O')
  {
    if (playerX == 0) {
        sem_wait(sem);
        randomPlay(gameboard, 'X');
        sem_post(sem);
    } else if (playerO == 0) {
        sem_wait(sem);
        randomPlay(gameboard, 'O');
        sem_post(sem);
    } else if(referee == 0){
        sem_wait(sem);
        winnerFound = victory(gameboard);
    } else {
        if (winnerFound == '-')
        {
          std::cout << "No winner yet\n";
        }  else if (winnerFound == 'Z') {
          std::cout << "The Game is a TIE\n";
          break;
        } else {
          std::cout << "\nWe have a Winner!!!!!!\n"
           << "Player "<<winnerFound << " has won the game\n";
           return 0;
        }
      }
    }
}*/


//*****************************
//****************Parent
for(int j = 0; j<25; j++) {
  for (i=0; i < 3; i++) {
    pid = fork();
    if (pid < 0){
      sem_unlink ("producer");
      sem_close(sem);
      std::cout <<"Fork Error \n";
    }
    else if (pid ==0)
      break;
    //std::cout<<"TEST  fork PID is: "<<pid <<"\n";
  }
  if (pid !=0) {
    //wait for child to exit
    while (pid = waitpid (-1, NULL, 0)) {
        if (errno == ECHILD)
          break;
    }

    std::cout <<"\n All children have exited. \n";

    //shared memory detach
    shmdt (gameboard);
    //shmdt (p);
    shmctl (shmid, IPC_RMID, 0);

    // cleanup semaphores //
    sem_unlink ("producer");
    sem_close(sem);
    // unlink prevents the semaphore existing forever //
    // if a crash occurs during the execution         //
    exit (0);
  }

  //********child
  else{
      sem_wait (sem);           // P operation //
      printf ("  Child(%d) is in critical section.\n", i);
      if (i == 0) {
        randomPlay(gameboard, 'X');
        std::cout << "Player X's turn\n";
      } else if (i == 1) {
        randomPlay(gameboard, 'O');
        std::cout << "Player O's turn\n";
      } else if (i ==2) {
        winnerFound = victory(gameboard);
        std::cout << "Referee is checking\n";
      }
      sleep (1);
      //*p += i % 3;              //increment *p by 0, 1 or 2 based on i //
      std::cout <<"Winnerfound is: " <<winnerFound;

      if (winnerFound == 'X' || winnerFound == 'O')
      {
        std::cout << "\nWe have a Winner!!!!!!\n";
        std::cout << "Player "<<winnerFound <<" won the game!\n";
        return 0;
      }  else if (winnerFound == 'Z') {
        std::cout << "The Game is a TIE\n";
        return 0;
      } else {
        std::cout << "No winner yet\n";
      }
    }
      //printf ("  Child(%d) new value of *p=%d.\n", i, *p);
      sem_post (sem);           // V / Signal operation //

      //exit (0);

}

  ///******START OF TRIAL 2

/*
  int N=5; // Number of elements for the array

  //int *ptr = mmap(NULL,N*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,0,0);

  int shm_id;    //shared memory id
  if((shm_id = shmget(IPC_PRIVATE, sizeof(int), SHM_R | SHM_W)) < 0){

    perror(NULL);

    printf("Can't Create Shared memory segment \n");

  exit(1);

  }
  int rows {5}, columns{5};
  char* name;
  int shm_fd = shm_open(shm_id, O_CREAT | O_RDWR, 0666);
  char gameboard[5][5];
  char* ptr = mmap(0, 5*5, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  gameboard = (char(*)[columns])mmap(0, rows*columns,  PROT_WRITE,   MAP_SHARED,  shm_fd, 0);

  if(ptr == MAP_FAILED){
   printf("Mapping Failed\n");
   return 1;
  }


    for(int i=0; i < N; i++){
     ptr[i] = i + 1;
    }

    printf("Initial values of the array elements :\n");
    for (int i = 0; i < N; i++ ){
     printf(" %d", ptr[i] );
    }
    printf("\n");

    pid_t child_pid = fork();

    if ( child_pid == 0 ){
     //child
     for (int i = 0; i < N; i++){
         ptr[i] = ptr[i] * 10;
     }
    }
    else{
     //parent
     waitpid ( child_pid, NULL, 0);
     printf("\nParent:\n");

     printf("Updated values of the array elements :\n");
     for (int i = 0; i < N; i++ ){
         printf(" %d", ptr[i] );
     }
     printf("\n");
    }

    int err = munmap(ptr, N*sizeof(int));

    if(err != 0){
     printf("UnMapping Failed\n");
     return 1;
    }
*/

  return EXIT_SUCCESS;
}

//******************FUNCTION DEFINITIONS*******************************


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
  int counter = 0;
  for(int row{}; row<5; row++)               //outter loop to move down rows
  {
    for(int cols{};cols<5; cols++)          //inner loop to move across columns
    {

      if(board[row][cols] != '-'&& board[row][cols] == board[row][cols+1])      //check for horizontal winner
        if(board[row][cols] != '-'&& board[row][cols] == board[row][cols+2])
          if(board[row][cols] != '-'&& board[row][cols] == board[row][cols+3]){
            std::cout << "horizontal\n";
            std::cout <<"Last winning square: " << row<<" " << cols+3;
            return winner = board[row][cols];}

      if(board[row][cols] != '-'&& board[row][cols] == board[row+1][cols])        //check for vertical winner
        if(board[row][cols] != '-'&& board[row][cols] == board[row+2][cols])
          if(board[row][cols] != '-'&& board[row][cols] == board[row+3][cols]){
            std::cout << "Vertical\n";
            std::cout <<"Last winning square: " << row+3 <<" " << cols;
            return winner = board[row][cols];}

      if(board[row][cols] != '-'&& board[row][cols] == board[row+1][cols+1])    //check for Right diagonal winnerFound
        if(board[row][cols] != '-'&& board[row][cols] == board[row+2][cols+2])
          if(board[row][cols] != '-'&& board[row][cols] == board[row+3][cols+3]){
            std::cout <<"R Diag\n";
            std::cout <<"Last winning square: " << row+3 <<" " << cols+3;
            return winner = board[row][cols];}

      if(board[row][cols] != '-'&& board[row][cols] == board[row+1][cols-1])    //check for Left diagonal winnerFound
        if(board[row][cols] != '-'&& board[row][cols] == board[row+2][cols-2])
          if(board[row][cols] != '-'&& board[row][cols] == board[row+3][cols-3]){
              std::cout <<"L Diag\n";
              std::cout <<"Starting square: " << row <<" " << cols;
              std::cout <<"\nLast winning square: " << row+3 <<" " << cols-3;
              return winner = board[row][cols];}
      if(board[row][cols] == 'X' || board[row][cols] == 'O'){
        counter++;
        //std::cout<<"counter++ Space is: "<<board[row][cols]<< " counter is: "<< counter <<"\n";
      }
      if(counter == 25)
        return winner = 'Z';  //magic character for tie condition
    }
  }
  return winner;
}
/*
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
}*/
void randomPlay(char board[][5], char player)
{

    int random_max = 5;
    int row = rand() % random_max;
    //std::cout << "Row in randomPlay" << row <<"\n";
    int col = rand() % random_max;
    //std::cout << "Col in randomPlay" << col <<"\n";
    while(board[row][col] !='-')
    {
      row = rand() % random_max;
      col = rand() % random_max;
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
