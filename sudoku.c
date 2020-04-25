#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#define VALID 1
#define INVALID 0

typedef struct node node_t;

typedef struct{
  int i, j;
}coords;

struct node{
  coords coord;
  node_t *next, *prev;
};

typedef struct{
  node_t *start, *end;
}path_t;

\\ CHANGE THE PUZZLE IN THIS ARRAY HERE

int puzzle[9][9] = {{0, 0, 0,     0, 7, 0,    1, 0, 0},
                  {0, 0, 0,     5, 6, 0,    0, 0, 0},
                  {0, 8, 0,     0, 2, 0,    0, 3, 0},

                  {0, 0, 0,     0, 0, 0,    4, 9, 0},
                  {0, 4, 0,     2, 5, 0,    0, 0, 8},
                  {5, 0, 0,     9, 0, 0,    0, 0, 6},

                  {4, 0, 6,     0, 0, 0,    0, 0, 0},
                  {2, 0, 0,     0, 0, 0,    0, 0, 0},
                  {7, 0, 0,     1, 9, 0,    8, 0, 0}
                };

             
int in_allowed(coords curr, coords *allow, int count);
int **create_puzzle(int dimension);
void solve_puzzle(int **puzzle, int dimension, coords *allow, int allow_count, int allow_pos);
int check_valid(int **puzzle, int dimension, int start_i, int start_j);
path_t *make_path(void);
path_t *insert_at_end(path_t *path, coords curr);
node_t *prev_pos(path_t *path, int i, int j);
void allowed(int **puzzle, int dimension, int *count, coords *allow_bounds);
void print_puzzle(int **puzzle);



int main(int argc, char *argv[]){
  int default_size = 9;
  int **newpuzzle = create_puzzle(default_size);
  path_t *path = make_path();

  for (int i=0; i<9; i++){
    if (i != 0){
      printf("\n");
    }
    if (i % 3 == 0){
      printf("\n");
    }
    for (int j=0; j<9; j++){
      if (j % 3 == 0){
        printf(" ");
      }
      printf("%d ", newpuzzle[i][j]);

    }
  }
  printf("\n");

  int *counter = malloc(sizeof(*counter));
  /////////////////////////////////////////////////////////////////////////

  coords *allow = (coords *)malloc(sizeof(coords) * 30);
  assert(allow != NULL);
  allowed(newpuzzle, default_size, counter, allow);

  int valid = check_valid(newpuzzle, default_size, 0, 0);

  if (valid){
    printf("Puzzle is valid!");
  }
  else{
    printf("Nope, yeet this outta here. ");
  }

  int allow_count = *counter, allow_pos = 0;

  printf("SOLVING PUZZLE\n\n");
  solve_puzzle(newpuzzle, default_size, allow, allow_count, allow_pos);
  ////////////////////////////////////////////////////////////////////////////

  valid = check_valid(newpuzzle, default_size, 0, 0);

  if (valid){
    printf("DONE! Printing puzzle...\n");
  }

  // Printing Solved Puzzle

  print_puzzle(newpuzzle);

  return 0;
}

void print_puzzle(int **puzzle){
  for (int i=0; i<9; i++){
    if (i != 0){
      printf("\n");
    }
    if (i % 3 == 0){
      printf("\n");
    }
    for (int j=0; j<9; j++){
      if (j % 3 == 0){
        printf(" ");
      }
      printf("%d ", puzzle[i][j]);

    }
  }
  printf("\n\n");

}

void allowed(int **puzzle, int dimension, int *count, coords *allow_bounds){

  int counter = 0;

  for (int i=0; i<dimension; i++){

    for (int j=0; j<dimension; j++){
      //printf("no allow i = %d   j = %d\n", i ,j);
      if (puzzle[i][j] == 0){
        //printf("setting to zero\n");
        //no_bounds[counter].i = 0;    // error lies in setting array
        //printf("pass");
        allow_bounds[counter].i = i;
        allow_bounds[counter].j = j;

        counter++;
      }
    }
  }

  *count = counter;

  //printf("finish not_allowed\n");

  return;
}

int in_allowed(coords curr, coords *allow, int count){
  for (int i = 0; i < count; i++){
    if (curr.i == allow[i].i && curr.j == allow[i].j){
      return VALID;
    }
  }

  return INVALID;
}

path_t *make_path(void){
  /* Initialises empty path for grid to backtrack */
  path_t *path = (path_t *)malloc(sizeof(path_t));
  assert(path != NULL);
  path->start = path->end = NULL;

  return path;

}

path_t *insert_at_end(path_t *path, coords curr){

  node_t *new = (node_t *)malloc(sizeof(node_t));

  assert(path != NULL && new != NULL);

  new->coord = curr;
  new->next = new->prev = NULL;

  if (path->end == NULL){
    path->start = path->end = new;
  }
  else{
    //printf("start i = %d  j = %d\n", path->start->coord.i, path->start->coord.j);
    new->prev = path->end;
    path->end->next = new;
    path->end = new;
  }

  return path;
}

node_t *prev_pos(path_t *path, int i, int j){
  node_t *temp = (node_t *)malloc(sizeof(node_t));
  temp = path->start;

  while (temp->coord.i != i || temp->coord.j != j){
    temp = temp->next;
  }

  //printf("temp i = %d   j = %d\n", temp->coord.i, temp->coord.j);
  //printf("prev temp i = %d   j = %d\n\n", temp->prev->coord.i, temp->prev->coord.j);

  return temp->prev;
}

int **create_puzzle(int dimension){

  int **newpuzzle = (int **)malloc(sizeof(int *) * dimension);
  for (int i = 0; i < 9; i++){
    newpuzzle[i] = (int *)malloc(sizeof(int) * dimension);

  }

  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      newpuzzle[i][j] = puzzle[i][j];
    }
  }

  return newpuzzle;


}

void solve_puzzle(int **puzzle, int dimension, coords *allow, int allow_count, int allow_pos){

  // Iterative Structure

  allow_pos = 0;

  while(allow_pos < allow_count){

    coords curr;

    curr.i = allow[allow_pos].i; curr.j = allow[allow_pos].j;

    //printf("curr i = %d   j = %d\n", curr.i, curr.j);

    puzzle[curr.i][curr.j]++;

    while (!check_valid(puzzle, dimension, 0, 0)){
      puzzle[curr.i][curr.j]++;
    }

    if (puzzle[curr.i][curr.j] > 9){
      puzzle[curr.i][curr.j] = 0;
      //print_puzzle(puzzle);
      allow_pos--;

    }
    else{
      //print_puzzle(puzzle);
      allow_pos++;
    }

  }

  return;
}

int check_valid(int **puzzle, int dimension, int start_i, int start_j){

  //printf("checking valid\n");
  //printf("start_i = %d  start_j = %d\n\n", start_i, start_j);

  if (start_i == dimension){
    return VALID;
  }
  //printf("checking valid\n");
  //printf("i = %d  \n j = %d\n\n", start_i, start_j);
  if (puzzle[start_i][start_j] != 0){

    if (start_i < 3 && start_j < 3){    //left top box

      for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){

          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if (start_i < 3 && (start_j >= 3 && start_j < 6)){ //top mid box
      for (int i = 0; i < 3; i++){
        for (int j = 3; j < 6; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if (start_i < 3 && (start_j >= 6 && start_j < 9)){   //right top box
      for (int i = 0; i < 3; i++){
        for (int j = 6; j < 9; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if ((start_i >= 3 && start_i < 6) && start_j < 3){ //mid left box
      for (int i = 3; i < 6; i++){
        for (int j = 0; j < 3; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if ((start_i >= 3 && start_i < 6) && (start_j >= 3 && start_j < 6)){  // mid box
      for (int i = 3; i < 6; i++){
        for (int j = 3; j < 6; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if ((start_i >= 3 && start_i < 6) && (start_j >= 6 && start_j < 9)){  // mid right box
      for (int i = 3; i < 6; i++){
        for (int j = 6; j < 9; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if ((start_i >= 6 && start_i < 9) && start_j < 3){   // left bottom box
      for (int i = 6; i < 9; i++){
        for (int j = 0; j < 3; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if ((start_i >= 6 && start_i < 9) && (start_j >= 3 && start_j < 6)){  // bottom mid box
      for (int i = 6; i < 9; i++){
        for (int j = 3; j < 6; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }

    else if ((start_i >= 6 && start_i < 9) && (start_j >= 6 && start_j < 9)){  // bottom right box
      for (int i = 6; i < 9; i++){
        for (int j = 6; j < 9; j++){
          if (puzzle[i][j] == puzzle[start_i][start_j] && (i != start_i || j != start_j)){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
    }



    for (int i = 0; i < dimension; i++){

      if (i == start_i){
        for (int j = 0; j < dimension; j++){
          if (start_j != j && puzzle[i][j] == puzzle[start_i][start_j]){
            //printf("i = %d   j = %d", i, j);

            return 0;
          }
        }
      }
      else{
        if (puzzle[i][start_j] == puzzle[start_i][start_j]){
          //printf("i = %d   j = %d", i, start_j);

          return 0;
        }
      }
    }
  }



  if (start_i < dimension - 1 && start_j == dimension - 1){
    check_valid(puzzle, dimension, start_i + 1, start_j = 0);
  }
  else if (start_j < dimension - 1){
    check_valid(puzzle, dimension, start_i, start_j + 1);
  }

}
