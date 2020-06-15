/*THIS PROGRAM CREATES A GAME OF LIFE AND SIMULATES THE CHANGE OVER A SET PERIOD OF TIME BY THE USER */

#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

char LIVE = 'X';//char which will  set life cells to 'X'
char DEAD = ' ';//char which will set dead cells to ' '

class OriginalCell
      {  private:
            HANDLE boarddisplay = GetStdHandle(STD_OUTPUT_HANDLE);//access rights
            COORD coordinates;//stores x and y coordinates of the life cell
                
         public:
             OriginalCell(int y,int x)//creates a life cell, setting the x and y coordinates
             {   
                 coordinates.Y=y;//
                 coordinates.X=x;
                 
             }
             void DisplayCell()
             { SetConsoleCursorPosition(boarddisplay,coordinates);//sets cursor on the console at the coordinates of the life cell
               cout<<LIVE;//displays 'X' to represent life
             }
      };


void generateOriginalLivecells(int y,int x)// //This function sets all the OriginalCell object parameters hence creates  a pointer of the OriginalCell class and prints out the 'X' representing life at that particular coordinate
{ OriginalCell *Cellpointer;//pointer to store cell
  Cellpointer=new OriginalCell(y,x);
  Cellpointer->DisplayCell();
}


char GetLiveValueAtCursorPosition(int row,int col)//This funtion returns the state of the cell on the console as a char ( either alive or dead
{   COORD position;
    position.X=col;//Takes in X and Y coordinates of the cell to determine what is printed on that cell in the console
    position.Y=row;
    char c = '\0';//variable used to store the actual value of char in cell on console
    CONSOLE_SCREEN_BUFFER_INFO con;//contions information about the screen buffer
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);//access rights
    if (hcon != INVALID_HANDLE_VALUE &&GetConsoleScreenBufferInfo(hcon,&con))//catches any errors in case of refusal of rights or inability to acess screen buffer info
    {
        DWORD read = 0;//Parameter of ReadConsoleOutputCharacterA and it recieves the number of characters actually read, I am using "DWORD" instead of "unsigned int" because it has the correct range and format of which Windows functions rely on. Incase of Mac or Linux use unsigned int
        if (!ReadConsoleOutputCharacterA(hcon,&c,1,position,&read) || read != 1)//Sets c to NULL in case cell is empty and "ReadConsoleOutputCharacterA(hcon,&c,1,position,&read)" actually stores in the char value in c
           {c = '\0';}
    }
    return c;
}

int checkneighbors(int r,int c)//this function counts the number of neigbours around each cell which are alive and returns that number. It takes in coordinates of the cell as r and c
{           // check the neighbors
            //will check each 8 neighbors and add to the counter if it spots an X
            //will return the final counter
            int neighbor_count = 0;//Stores number of neighbours
            //ALL CONDITIONS ARE IFs TO ENSURE THAT EVERY STATE OF EVERY NEIGHBOUR OF THE CELL IS CHECKED
            if (GetLiveValueAtCursorPosition(r,c-1) == LIVE)//Checks if the char at the cell with coordinates one column to the left is equal to LIVE
               {neighbor_count++;}
            
            if (GetLiveValueAtCursorPosition(r-1,c-1) == LIVE)//Checks if the char at the cell with coordinates one column to the left and row above is equal to LIVE
                {neighbor_count++;}
            if (GetLiveValueAtCursorPosition(r-1,c) == LIVE)//Checks if the char at the cell with coordinates at row above and same column is equal to LIVE
                {neighbor_count++;}
            if (GetLiveValueAtCursorPosition(r-1,c+1) == LIVE)//Checks if the char at the cell with coordinates one column to the right and row above is equal to LIVE
                {neighbor_count++;}
            if (GetLiveValueAtCursorPosition(r,c+1) == LIVE)//Checks if the char at the cell with coordinates at  same row and column to the right is equal to LIVE
                {neighbor_count++;}
            if (GetLiveValueAtCursorPosition(r+1,c+1) == LIVE)//Checks if the char at the cell with coordinates one column to the right and row below is equal to LIVE
                {neighbor_count++;}
            if (GetLiveValueAtCursorPosition(r+1,c)== LIVE)//Checks if the char at the cell with coordinates at row below and same column is equal to LIVE
                {neighbor_count++;}
            if (GetLiveValueAtCursorPosition(r+1,c-1)== LIVE)//Checks if the char at the cell with coordinates one column to the left and row below is equal to LIVE
                {neighbor_count++;}
                                  
            return neighbor_count;
}


void generate(int r, int c)//this function sets cells on the console as life or death depending on the rules of the game of life. Taking in intergers r and c for coordinates of current cell
{  /*
    Any live cell with fewer than two live neighbors dies, as if caused by under-population.
    Any live cell with two or three live neighbors lives on to the next generation.
    Any live cell with more than three live neighbors dies, as if by over-population.
    Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
    */
    //   call checkneighbors with current cell and current row and column
    //   apply rules and use the result to add either a new cell or leave the current cell alone 

    
            HANDLE board = GetStdHandle(STD_OUTPUT_HANDLE);//access rights
            int neighbors = checkneighbors(r,c);
            COORD coordinate;
            coordinate.X=c;
            coordinate.Y=r;
            SetConsoleCursorPosition(board,coordinate);//sets cursor position at current cell in the console
            if (GetLiveValueAtCursorPosition(r,c) == LIVE && neighbors < 2)// Conditional statements below cout 'X' or space depending on situation and following game of life rules
            {
                cout<<DEAD;
            }
            else if (GetLiveValueAtCursorPosition(r,c)== LIVE && (neighbors == 2 || neighbors == 3))
            {
                cout<< LIVE;
            }
            else if ( GetLiveValueAtCursorPosition(r,c)== LIVE && neighbors > 3)
            {
                cout<< DEAD;
            }
            else if ( GetLiveValueAtCursorPosition(r,c)== DEAD && neighbors == 3)
            {
                cout<<LIVE;
            }
            else{cout<<DEAD;}
        
    

}


int main()
{   int BOARDCOLS;//Both variables  BOARDCOLS,BOARDROWS store size of column and row 2d console as an array
    int BOARDROWS;
    char option='B';
    int time,initial=0;//Stores the number of iteration of the while loop simulating it as years
    //Creates board to enable the generation of the cellular automata
    HANDLE board = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordin;
    cout<<"THIS PROGRAM GENERATES A SIMULATION OF CONWAYS GAME OF LIFE"<<endl;
    cout<<"CHOOSE FROM THE OPTIONS:Type 'A' to see the default simulation of the game of life or Type 'B' to create your own board with its life cells using a file"<<endl;
    cin>>option;
    switch(toupper(option))
    {
      case 'A':
            {   BOARDCOLS=102;//Setting the dimensions of the board i.e I didnot use the entire console window for speed
                BOARDROWS=25;
                cout<<"HERE IS A DEFAULT SIMULATION OF THE GAME OF LIFE USING TWO POPULATIONS OVER A PERIOD OF TIME IN YEARS:"<<endl;
                cout<<"ENTER THE NUMBER OF TIMES WHICH YOU WANT THE SIMULATION TO RUN:";
                cin>>time;
                //First population is created by setting their coordinates on the console terminal  cells and displaying LIVE
                
                generateOriginalLivecells( 15, 43);
                
                generateOriginalLivecells( 15, 44);
                
                generateOriginalLivecells( 14, 44);
                
                generateOriginalLivecells( 16, 43);
               
                generateOriginalLivecells( 17, 44);
                
                //Second population
                
                generateOriginalLivecells( 12, 70);
                
                generateOriginalLivecells( 13, 71);
                
                generateOriginalLivecells( 11, 72);
                
                generateOriginalLivecells( 11, 71);
                
                generateOriginalLivecells( 13, 73);
                
                while(initial<=time) 
                  {  for (int r = 9; r < BOARDROWS; r++)//Nested for loop to enable iteration through 2d console array. r starts at 9 and c starts at 30 so that the simulation is displayed in the middle of the terminal
                    {   
                        for (int c = 30; c <BOARDCOLS; c++)
                        { generate(r,c);
                        }
                    }
                    coordin.X=30;//resets the cursor below the simulation after evry iteration of the while loop so that "press any key to continue" is not displayed on the simulation 
                    coordin.Y=26;
                    SetConsoleCursorPosition(board,coordin);
                    initial++;
                  }
                break;
            }
      case 'B':
            {   vector<string>cell;//This vector stores the original board from the file as a 2d array
                ifstream Input;//This object will be used to open the file from which the board input is taken
                string Filename,colstring,rowstring,boardperline;//The variable "Filename" stores in the name of the file to be used as input, the variable "colstring" stores the total number of columns of the board as a string, the variable "rowstring" stores the total number of  rows of the board as a string, "boardperline" takes in each row of the board as a string
                char CharUsedToRepresentLifeInFile;//Stores in the character that is used to represent a life cell on the board in a the file 
                cout<<"ENTER THE NAME OF THE FILE CONTAINING THE ORIGINAL DISPLAY OF THE BOARD AND ITS DIMENSIONS:";
                cin>>Filename;//stores the name of the file containing the board 
                Input.open(Filename+".txt");
                Input>>colstring;//takes in the number of colums of the board to be converted to int as a string 
                Input>>rowstring;//same but with rows
                Input>>CharUsedToRepresentLifeInFile;
                BOARDCOLS=stoi(colstring)+30;//converts the dimensions of the board from string to int and stores them in BOARDCLS
                BOARDROWS=stoi(rowstring)+9;
                cout<<"HERE IS A SIMULATION OF GAME OF LIFE AS ORIGINALLY GOTTEN FROM THE FILE NAMED '"<<Filename<<"'"<<endl;
                cout<<"ENTER THE NUMBER OF TIMES WHICH YOU WANT THE SIMULATION TO RUN:";
                cin>>time;
                while(Input>>boardperline)
                   {  cell.push_back(boardperline);}//While loop stores the board as rows from boardperline to the vector cell. Also here boardperline will be used as a C-string hence as an array of characters
                     
                for(int j=0;j<cell.size();j++)
                    {    for(int k=0;k<cell[j].size();k++)
                         { if( cell[j][k]==CharUsedToRepresentLifeInFile) //Iterates through the 2d vector and checks if the elements at a particular coordinate equals to the value of CharUsedToRepresentLifeInFile and sets the cursor to that characters coordinates on the console terminal and displays a life cell there      
                              {  
                                 generateOriginalLivecells( j+9, k+30);
                              }
                       }
                    }
                while(initial<=time) 
                  {  for (int r = 9; r < BOARDROWS; r++)//Nested for loop to enable iteration through 2d console array. r starts at 9 and c starts at 30 so that the simulation is displayed in the middle of the terminal
                    {   
                        for (int c = 30; c <BOARDCOLS; c++)
                        { generate(r,c);
                        }
                    }
                    coordin.X=30;//resets the cursor below the simulation after every iteration of the while loop so that "press any key to continue" is not displayed on the simulation 
                    coordin.Y=BOARDROWS+1;
                    SetConsoleCursorPosition(board,coordin);
                    initial++;
                  }
                Input.close();
                break;
            }
      default:
             cout<<"SORRY INVALID INPUT PLEASE TRY AGAIN"<<endl; 
    }
        
    return 0;
}




