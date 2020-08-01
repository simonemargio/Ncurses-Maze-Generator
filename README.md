# Database RisiKo

## Index
### Documentation
1. [Introduction](#Introduction)
  * [Content of the documentation](#Content-of-the-documentation)
    * [Developer manual](#Info-developer-manual)
    * [User Manual](#Info-user-Manual)
  * [Commissioned project](#Commissioned-project)
  
### Developer manual
1. [Composition](#Composition)
  * [Directives](#Directives)
  * [L_Struttura](#L_Struttura)
    * [Main structure](#Main-structure)
    * [Node structure](#Node-structure)
    * [Coordinate node](#Coordinate-node)
    * [Weight structure](#Weight-structure)
    * [Structure estimates](#Structure-estimates)
    * [Predecessors structure](#Predecessors-structure)
  * [L_Heap](#L_Heap) 
    * [Heap management structure](#Heap-management-structure)
    * [Tree node](#Tree-node)
2. [Search algorithms](#Search-algorithms)    
  * [Overview](#Overview)
  * [L_Astar](#L_Astar)
  * [L_Dijkstra](#L_Dijkstra)
3. [Maze generators](#Maze-generators)    
  * [Overview](#Overview)
    * [Dead ends](#Dead-ends)
    * [L_Prim](#L_Prim)
    * [L_Dfs](#L_Dfs)
    * [L_Recursive](#L_Recursive)
    * [L_Divide](#L_Divide) 
  * [L_Maze](#L_Maze)
4. [Utility and management](#Utility-and-management)    
  * [Generic libraries](#Generic-libraries)
    * [L_Utility](#L_Utility)
    * [Menu management](#Menu-management)
    * [Communication with the user](#Communication-with-the-user)
    * [Use of color](#Use-of-color)
    * [Animations](#Animations) 
    * [Error handling](#Error-handling) 

### User Manual
1. [Overview](#Overview)
  * [Compilation and launch](#Compilation-and-launch)
  * [Interfaces](#Interfaces)
  * [Executable operations](#Executable-operations)
    * [Modify maze](#Modify-maze)
    * [Search for the minimum path](#Search-for-the-minimum-path)
    * [Viewing cells explored](#Viewing-cells-explored)

 
# Introduction
## Content of the documentation

The following documentation is divided into two parts.

The first part is intended for developers and those who are willing to understand how the program works at the project level.

Alta, on the other hand, is intended for the end user and for those who want to access the correct use of the program, taking advantage of all its features.

It is possible to separate the two manuals according to the type of reader to which it should be destined, or consider the use of the entire documentation.


### Info developer manual

The developer manual refers to those who intend to examine the mechanisms and systems that revolve around the commissioned project.

The analysis takes into consideration the files that make up the program, giving a description of their content and underlining certain implementation details adopted.

The use of all data structures and their interactions with the rest of the program are then shown.


### Info user Manual

The user manual includes the analysis from the functional point of view of the program.

The correct compilation and execution of the program is shown. We then move on to the interfaces and their correct use in more detail.


## Commissioned project

Create an application in C, using the **ncurses** library to:

Randomly generate, through menu selection, mazes that exploit the following generators:
 * PRIM inspired generator.
 * Generator inspired by DFS.
 * Recursive generator (inspired by DFS).
 * Recursive split generator.
 
We then proceed to visualize the labyrinth and modify it interactively (moving with the keyboard or mouse to select walls to be eliminated or corridors to be closed).

Give the possibility of being able to search the minimum route from a source to a destination.

Allow interactive selection of source and destination with on-screen display of the route.

Video animation of a character following the path and possibility to graphically display (for example with different color colori erent colors) the cells and esploractively explored by the search algorithm of the minimum selected path.


# Developer manual
## Composition

The program consists of the union of thirteen source files with as many headers. Each of these constitutes a fundamental role in the overall functioning of the application.

Some of these have been developed to be exported and used individually on di ﬀ erent applications; others, however, require the joint export of multiple sources for their correct functioning.

Before proceeding with their analysis, the *#defines* present and their use are de ﬁ ned below.


## Directives
All the directives are specified within the *L_Utility.h* file.

Are present:
 * RIGA and COLONNA: Specify the row and column dimensions of the matrix / labyrinth that will be generated.
For a correct and elegant video display of the labyrinth it is recommended to set column values equal to twice those of row.
By default their values are set to: *twenty* rows and *forty* columns.
 * OFFSET:Both; *OFFSET_X* and *OFFSET_Y* constitute the row and column index used as a starting point to carry out the maze design which will then be printed on the screen.
Changing these values leads to a displacement of the representation of the labyrinth.
To create a sense of space, by default their values are set to: *two* for the offset X and *two* for the offset Y.
 * DELAY:Numerical value expressed in microsecond interval. It is used to create the e etto ect of animation and movement of a character that follows the calculated path.
Since the value is used through the use of the *usleep* function, which suspends the execution of the calling thread, it is good practice not to set it to very high values.
Equally much lower values would lead to an incorrect display of the animation.
By default the set value is: *one hundred and thirty thousand*.
 * INFINITO: It is the only directive present in the *L_Dijkstra* header.
This directive consists more of its representative status rather than its modification. The search algorithm for the minimum path associated with it takes into consideration the use of values tending towards infinity.
Its default speci ﬁ c value is: *minus one*.
This value must not be misleading. A possible implementation could use the maximum value that can be represented by the machine, but a more elegant solution has led to the use of a numerical value that can never be "reached" by the algorithm; more precisely from the arches and their possible sum.
You will then see the use in detail of this value associated with the two minimum path search algorithms implemented.


## L_Struttura
It represents the driving force at the structural level of the whole program. All the structures used are specified inside, with the exception of the *heap* present in the *L_Heap* file.

The primary management structure is:


### Main structure
```c
struct struttura_principale {
   struct struttura_nodo ∗∗ matrice ;
   int rsize ;
   int csize ;
   int nsize ;
   WINDOW ∗wMenu;
   WINDOW ∗wOplab;
   struct struttura_gestione_heap ∗StrHeap ;
};
```
This structure is allocated to the execution of the program and has the task of maintaining the main information that revolves around the application. Among these we find:
 * *matrice*: it constitutes a pointer to the representative matrix of the labyrinth. In the next structure the types of *struttura_nodo* elements that make up each single cell of the matrix will be specified.
 * *rsize, csize*: row and column of the matrix. The values are set by the *RIGA* and *COLONNA* directives in *L_Utility*.
 * *nsize*: used as a convenience variable. It performs two functions:
    1. Used as a counting variable of the elements present in the *queue* structure for the generation of the labyrinth using the *Prim* algorithm.
The latter uses a queue with random selection of an element on which various operations can then be performed; *nsize* allows you to randomize this numerical value based on the maximum number of elements in the queue.
    2. Used as a discriminant variable to keep information on the type of search algorithm of the minimum path used. This value is used in the display functionality of the cells explored by the search algorithm. As we will see later, there are two search algorithms: **Dijkstra and A***; as the Dijkstra research structure involves the use of **all** nodes within a heap structure, making the visited nodes equal to the entire set of nodes present, it was preferred to restrict the possibility of viewing the cells and actually explore only after the use of the A* algorithm.

```c
if(P−>nsize==2)
{
	F_celle_esplorate(P);
	P−>nsize=0;
	Stampa("Select an option from the menu.");
	break;
}
else
 if(P−>nsize==0) Stampa ("You have to to execute before 'Research route minimum'. Awards sending for to continue.");
 else Stampa("The display is only valid for A star, press enter to continue.");
}
```

You can see how the value *zero* can be associated with the value nsize, indicating that the search for a route has not yet been carried out. The *two* value speci ﬁ es that the search algorithm used is **A***, and then the explored cells are displayed.

Finally, the value *one*, expressed in this case by the final *else*; specifies that the algorithm used previously is that of **Dijkstra** and that therefore it is useless to view the cells explored.

The ability to view all the cells explored by the latter algorithm has not been completely removed. In fact, in the function *F_dijkstra* in *L_Dijkstra* it is possible to enable the maintenance of the information of the cells explored as specified below:

```c
/∗
   Enable if you also want the information of the cells visited by Dijkstra
   M∗∗matr=P−>matrice;
   matr[r][c].visitato=2;
∗/
```

Obviously the use of these instructions must lead to an appropriate modification of the nsize value associated with the algorithm.
 * *wMenu*: pointer to the window generated for the two main menus of the application.
 * *wOplab*: pointer to the window generated for the menu for managing the functions that can be performed on the labyrinth.
 
 *More information on the menus in the user manual.*
 * *StrHeap*: constitutes a pointer to the general management structure of a heap-type tree.

The creation, therefore, of a single structure containing all the information for managing the program has allowed a reduction and harmoniousness of the parameters passed to the existing functions. These receive as input only the pointer of the entire main structure and, based on the function, operate by taking the information they need.

### Node structure
```c
struct struttura_nodo {
   int murcorr;
   int visitato;
   struct struttura_peso ∗pUP;
   struct struttura_peso ∗pDW;
   struct struttura_peso ∗pSX ;
   struct struttura_peso ∗pDX;
   struct coordinate_nodo∗inf;
};
```
It contains the following information:
 * *murcorr*: expresses the state of a cell. It can be a wall (numerical value *zero*) or a corridor (numerical value *one*).
 * *visited*: it contains the information about the cell visit. *Three* values can be associated with it:
    1. Value *zero*: indicates that the cell has never been visited. trice this value is set to *zero*.
    2. Value *one*: the cell has been visited and is part of the set of cells that make up the minimum path.
    3. Value *two*: this value is associated with a cell by the algorithm A *, or as seen also for Dijkstra, to indicate the true state of the cell being explored, i.e. cell to which all its neighbors have been expanded.
    
```c
...
{
   Albero p=pred[rA][cA].pred;
   F_stampa_percorso (matr,pred,rP,cP,r,c);
   matr[rA][cA].visitato=1;
}
...
```

You can see how the reconstruction algorithm of the path *F_print_path* in *L_Utility*, works by associating the cell of a certain row and column with the value *one*. A subsequent reading of each cell containing this value allows to be able to print on the screen the minimum path, if any.

In summary, the search algorithm places the cells explored equal to *two*, subsequently the search for the minimum path associates the value *one* and, if this path is present, the two associated values allow to discriminate the cells explored from those belonging to the path found.

 * *pXX*: with *XX* equal to: *UP, DW, SX, DX*. They constitute an arch between two adjacent cells depending on the location in one of the four cardinal points. The pointer is connected to the structure containing the information of the *weight* of the arc between the two cells. This constructive choice allows to **keep only** and exclusively the information on the cells that are effectively connected to each other. Cells with adjacent walls will have the pointer relative to NULL while those connected to each other will point to the same structure that indicates their weight.

 * *inf*: pointer to the structure, shown afterwards, which maintains the information of the coordinate of the reference cell. This information is used for the following purposes:
    1. During the use of the generation of a labyrinth using the *Prim* method, since *"random"* cells are inserted in the queue, and the same selection of these cells from the tail is randomized; the information of the row and column on which the taken cell is located is associated with each cell in the queue. Subsequently, access to this structure allows you to operate without problems with the selected cell and search for its neighbors.
Similar speech also for the generation of a labyrinth using the DFS methodology.
    2. Maintenance of row and column information of a cell selected interactively by the user.
Having already available this structure, instead of returning and operating with the event containing the information on which the user clicked, it was preferred to use inf to save this *inf* and to be able to return it to any function without problems.

In this structure, in addition to the row and column, there is an additional parameter which will be discussed later.
In summary, the node structure contains the "static" information that is always used by any function operating on a cell, and pointers to appropriate structures that are allocated and connected only as needed.

### Coordinate node
As described above, this structure maintains information about a particular cell.

```c
struct coordinate_nodo {
   int r;
   int c;
   int p;
};
```
Having already talked about the value of *r-ow* and *c-olumn*, let's know about the use of *p*.
This value is used only by the *Prim* type labyrinth generation algorithm. The use is due to the maintenance of the *p-redecessor* cell to which the excavation is being carried out.
More in detail, the connection of two cells using the *F_dig_to* function in *L_Maze* takes a cell **A** taken from the *queue* whose coordinates are known by accessing the node coordinate structure, while the other cell **B** is its adjacent chosen in a totally random way.
In order to be able to connect the two cells, a numerical value associated with *p* is then inserted in the inf structure of **A** which identifies the exact position in which cell **B** is located.

For example, if the *p* value associated with cell **A** is *two*, it means that cell **B** is located to the left of cell **A** and consequently the appropriate pointers *pSX* of **A** will be connected with *pDX* of **B**.

### Weight structure
```c
struct struttura_peso {
   float peso;
};
```
Contains the weight between two nodes / cells of the matrix. The weight value is randomly generated.

### Structure estimates
The creation of this structure also applies to the subsequent structure, *predecessor structure*. Both these structures, in fact, are allocated as **matrices** in order to associate each cell with its relative information of estimates or predecessors.

Their allocation occurs when the user searches for a route using one of the search algorithms. Subsequently, while the estimate matrix will be deallocated, that of the predecessors will be inserted in the *heap* structure, of which we will talk about it in *L_Heap*, to then be used for the search for a possible minimum path.

```c
struct struttura_d{
   float stima;
};
```
In this case, the information of the estimate calculated based on the search algorithm used is present.

### Predecessors structure
```c
struct struttura_p
{
   void∗pred;
};
```
It associates the memory address of the selected cell. The use of the *void* type allows the expansion of the type of information that can be connected and subsequently examined.

## L_Heap
Contains all the functions and procedures for managing a *heap* tree. The use of this file is necessary in order to take advantage of the minimum path search algorithms.
Both **Dijkstra** and **A*** use the *heap* tree to ensure constant access to the node information with minimal estimate.

Within this library, therefore, there are all the mechanisms that allow the creation and management of the tree. Obviously, certain changes have been made to better adapt the procedures to the use of search algorithms.

The main change is due to the use of the *INFINITO* directive. As seen, it represents the numerical value *minus one* that must be managed correctly.

An example can be taken from the first verification and carried out by the heap state maintenance algorithm, *F_heapify*:

```c
{
...
if ((priorita_i==INFINITO && priorita_l !=INFINITO) || (priorita_l !=INFINITO && priorita_i !=INFINITO && priorita_l<priorita_i)) mas=l;
else mas=i;
};
```
The function checks which of the parent nodes **i** and left child **l** contains the smallest estimate value. Since *fewer one* would always lead to having knots with this value at the top of the tree, the verification conditions have been rewritten to avoid this situation.

In the example, the first part of the condition considers the parent node with an *INFINITO* estimate while the child node with a numerical estimate. The condition is necessarily true since only the child has a valid estimate, and therefore is placed as "maximum".

The second one represents the real condition of heap verification. It can obviously only be assessed when the two reference nodes are values of estimates not equal to the *INFINITO*.

Other procedures such as: *F_insert_element_key* or *F_decrease_key_shaft* have undergone similar changes.

Further considerations to be made concern the construction of the heap. Since the same definition of this tree must lead to a representation of the type of *complete* tree, the construction of the heap takes place through the use of a binary mask that allows the insertion of each node from left to right, level by level.

```c
...
for ( k=0;k<DIMARRMASK; k++)
{
   int mask = 1<<k;
   int mask_n=n & mask;
   int bit_preso=mask_n>>k;
   bits[k]=bit_preso;
}
...
```

In this piece of code we proceed to "convert" the numerical value **n**, indicating the element to be inserted, in its binary representation.

*DIMARRMASK*, or the number of cells of the *bits* vector, was not mentioned in the directives since it is part of a numeric value (*thirty two*) completely acceptable and therefore uselessly modifiable.

### Heap management structure
As for the *main structure*, this also represents the main structure containing all the information to be able to operate with a heap tree.

```c
struct struttura_gestione_heap
{
   void∗struttura;
   int heapsize;
   struct struttura_d∗∗d;
   struct struttura_p∗∗p;
};
```
The following information is present:
 * *structure*: points to the root of the heap tree.
 * *heapsize*: contains the number of elements that make up the tree.
 * *d*: contains the pointer to the estimate matrix.
 * *p*: matrix of predecessors.

As already specified in the *Estimation Structure*, both the *d* and *p* matrices are allocated by the search algorithm, and since they use the use of heaps, it was preferred to connect the two matrices to this structure, and then operate on it.

This structure is the one that is connected to the main management structure already seen in *L_Structure*.

### Tree node
```c
struct struttura_nodo_albero
{
   int r;
   int c;
   void∗stima;
   struct struttura_nodo_albero∗ptrDx;
   struct struttura_nodo_albero∗ptrSx;
};
```
Represents a single node of the heap tree.
 * *r, c*: constitute the row and column of the cell of the reference labyrinth. Also in this case, as already seen, the maintenance of such information is fundamental.
In fact, during the use of one of the two search algorithms, the insertion of nodes/cells follows a precise logic dictated by the algorithm, but maintaining the *min-heap* structure can lead to a modification of the elements within the tree . It is therefore necessary to keep the information of the row and column of the cell on which to then operate.
 * *stima*: contains the cell **address** in the estimate matrix *d*.
 In this way it is possible to create a single association, *one by one*, between a cell of *r-ow c-olumn* and its calculated estimate. Therefore access to this value changes the estimate matrix.
 * *ptrDx,...Sx*: constitute the appropriate children of the node.

## Search algorithms
## Overview
The construction of labyrinths with *always* positive weights has allowed us to take advantage of the search algorithm of the minimum **Dijkstra** walk. A good but not entirely e ﬃ cient choice if applied to the use of graphs as mazes. The second type of algorithm **A*** therefore enters into play, which allows a completely efficient heuristic search by exploiting the Manhattan distance as the heuristic function *H*.
The latter plays a fundamental role in that, since the labyrinth displacements are restricted to only the four cardinal points without counting diagonal displacements, it enjoys both the **eligibility** and **consistency** properties.

The combination of these two characteristics associated with the heuristic function allowed the implementation of the A* algorithm by completely eliminating the set of nodes that could be fished in the *closedset*, bringing the algorithm closer to the same methodology as other search algorithms (such as BFS and Dijkstra himself) who; once you have *"closed"* a summit, you are **absolutely** sure that it will be forever. Getting to this summit, therefore, can only be the shortest distance found.

## L_Astar
The algorithm was divided into two procedures. The first provides for the initial initialization phase of the algorithm.

```c
void F_init_Astar ( Str P, int r, int c,int rA,int cA)
{
   Albero T=NULL;
   Dd ∗∗d=F_alloca_d(P);
   Pp∗∗p=F_alloca_p(P);
   StructHeap Heap=F_alloca_struttura_heap(P,d,p);
   d[r][c].stima=F_distanza_manhattan(r,c,rA,cA);
...
   F_Astar(P,Heap,r,c,rA,cA);
...
}
```
The procedure takes as input the main structure *P*, row and column of the source cell and row and column of the arrival cell.
The *tree* instruction and *four* create the reference matrices for estimates and predecessors. We then move on to the construction of the Heap management structure.

At the end of these phases, the function **F** is calculated.
Recall that in the algorithm there are **three** types of functions:
   
   1. *H*: corresponding to the heuristic function adopted on the basis of the reference domain of the commissioned problem. In this case it is the distance to Manhattan.
   
   2. *G*: estimate of the path from the chosen source to the reference node.
   
   3. *F*: the union of G + H.
    
In this case, since the estimate from the source to itself corresponds to the value *zero*, in instruction *seven*, the function *F = G + H* becomes *F = 0 + H*, thus calculating only the heuristic function. After the initialization part, we move on to the actual algorithm.

*For all the implementation details you can follow the source code, in these pages only the most significant elements are shown.*

The algorithm has been structured in such a way as to make it contained in its operations. In particular, the search for the neighbors of a specific node taken from the heap takes place by saving the information of the neighbor and then subsequently processing it.
```c
...
switch(i)
{
    case 0:
    // ADJ DX
    if(cX<P−>csize−1)
    {
      y=F_coord_nodo_dx(P,rX,cX);
      if(y−>murcorr !=0)
      {
       rY=rX;
       cY=(cX+1);
       peso=matr[rX][cX].pDX−>peso;
   }
}
break;
...
```
By means of a cycle with variable **i** which operates on the number of the *four* possible neighbors of a node **X** taken from the heap, the verification of the existence of an adjacent on the right takes place as in the example above.

The condition of row *five* verifies that the column coordinate of **X** is less than the total size of the columns of the matrix *minus one* (remember that the first element has zero index).

If the condition turns out to be true, then there will necessarily be an adjacent to the right of **X**.

Its coordinates are obtained through instruction *seven* and it is verified that the adjacent **Y** is not a wall.

Once this condition is also overcome, then the coordinates of the adjacent **Y** and the weight connecting the two arcs **X–>Y** are saved.

You are therefore ready to evaluate the saved information.
```c
...
cost=d[rX][cX].stima+peso+F_distanza_manhattan(rY,cY,rA,cA);
if(matr[rY][cY].visitato!=2)
...
if((d[rY][cY].stima==INFINITO && indice!=−1) || (indice!=−1 && cost<d[rY][cY].stima))
{
   // decrease
}
...
```
If the weight calculated in instruction *twelve* is a positive value then the function **F** is calculated on the adjacent **Y** taken.

The *cost* will be given, as already said, by the sum of the function:
*G(y)*: which represents the cost *G(x)*, or the reading of the estimate that occurs in instruction one; added to the weight of the arc between **X** and **Y** previously saved.

H *(y, destination)*: given by the heuristic function that takes the coordinates of the adjacent node **Y** and of the **chosen destination node**.

Once the *cost* has been calculated, the condition of instruction *three* is verified. 
It represents what has been speciﬁed in the Chapter *Overview*, i.e. the control relating to the presence or absence of the adjacent node **Y** in the *closed-set*. If the visited value is equal to *two* then node **Y** is already present in this set and therefore it is not re-expanded, but the next adjacent one is calculated.
If not, condition *five* is verified.

Since the *closed-set* set is specified through the use of the value *two* in the node taken into consideration while the *open-set*, that is the set of nodes encountered but not yet expanded, is initially composed only of the source node; it was decided to represent the set of estimates of all the other nodes not yet reached at *INFINITO*.

The last condition to be verified, therefore, breaks down into two checks.  
It is also worth noting the use of the index variable. 
It contains a numerical value that represents the exact *index*, inside the heap, of the adjacent node **Y**. If such adjacent is already present in the tree, then its index will be positive, otherwise it will be equal to *minus one*.

The condition to be verified leads, if so, to the execution of a *decrease key* since it has been found:
according to the first condition, the adjacent node had not yet been reached, therefore its estimate is equal to *INFINITO*; and the adjacent **Y** is present in the tree.  
Or the adjacent **Y** is present in the tree but the new calculated cost is **better** than what was already present.

Once this condition has been checked, go to the *decrease key* or in case of a negative evaluation of instruction *five*, proceed to inserting the adjacent node **Y** in the heap.

## L_Dijkstra
Dijkstra, like A*, has an initial initialisation procedure and subsequently proceeds with the execution of the algorithm.
```c
void F_init_dijkstra(Str P,int r,int c)
{
    Dd **d=F_alloca_d(P); 
    Pp **p=F_alloca_p(P); 
    StructHeap Heap=F_alloca_struttura_heap(P,d,p);

    F_crea_heap(Heap,P,r,c);

    F_dijkstra(P,Heap);
    ...
}
```

Here too we have the creation of the matrices of estimates and predecessors. The substantial di ﬀ erence with the initialization of A* is that Dijkstra proceeds to create a heap tree containing **all** the nodes/cells of the matrix that are **corridors**.
```c
switch (i){
    case 0: // ADJ DX
        if(c<P->csize-1){   
            v=F_coord_nodo_dx(P,r,c);
            if(v->murcorr!=0){   
                F_relax(Heap,u,r,c,r,c+1,v->pSX->peso);
            }
        }
    break;
...
```
After extracting the element with lesser estimate **U**, it can be seen that the search for adjacent nodes **V** is completely identical to the algorithm A*.  
Dijkstra proceeds, having checked all the conditions, to relax the arch between **U–>V**.  

The relaxation procedure is passed:  
 * *u*: is the node taken from the heap, if the *decrease key* procedure takes place then the node will be inserted as a predecessor node.
 * *r, c*: row and column of the cell to which *u* refers.
 * *r, c-1*: row and column of the adjacent node *v*. In this case the adjacent *left*.
 * *weight*: the weight between the two nodes *u* and *v*.
 
 We then move on to the relaxation function.  
```c
if(dU!=INFINITO){
    if(dV==INFINITO || dV > dU + arco)
    {
       // decrease
    }
} 
```
The first condition examines the estimate that node *u*, called **dU**, can contain. You should not be deceived about this verification because, due to the structure of the algorithm and the conditions of relaxation, the extraction of the root in the heap **always** contains a node with an estimate value necessarily different from *INFINITO*.

This is true as long as **all** the cells that are **corridors** are reachable from the chosen source node.   
But, since the application provides the possibility to modify the labyrinth run-time, it is possible *to "wall"* the corridor cells. Since, as mentioned, in the initialization of the algorithm, the built tree contains all the corridor nodes; if the user searches for a route that includes the *"walled"* cell as a starting point or destination, there will necessarily be an node with *INFINITO* estimate in the heap to which it will not be possible to associate an estimate, being unreachable.  
This condition, therefore, allows to consider only and exclusively nodes reachable from the source.

The second condition, on the other hand, checks whether it is necessary to perform a *decrease key*. The conditions are trivial. The decrease is performed if the adjacent node has an *INFINITO* estimate or has an estimate greater than the sum of the estimate of *u* plus the arc between the two.

## Maze generators
## Overview
Each generation algorithm has its own file, but all of them are dependent on the *L_Maze* file which incorporates the excavation and verification functions used by the generators.  
As described in the *commissioned project*, *four* types of labyrinth generators have been implemented. All, except for the recursive subdivision generator, operate on a matrix consisting of only walls and then carry out an excavation operation. The recursive subdivision instead exploits an initial matrix made up of all corridor-type cells.  
The allocation and construction of the matrix occurs only the first time that one of the *four* types of generators is chosen.
```c
if(P->matrice==NULL) F_alloca_struttura_matrice(P);
else { F_dealloca_archi_matrice(P); F_inizializza_matrice(P);}
```
The following times, if you want to change the type of generator, all the arcs of the matrix are eliminated and subsequently initialized (visit values and other information are restored to the default state).  
All the generators have been adapted to represent a labyrinth where a single cell can play one of two roles: wall or corridor. For example, if you consider the *Prim* type generator, you can see how its maze creation method allows you to dig a **single** unit from one cell to another.  
This type of generation would therefore lead to having **all** the cells of the matrix as **corridors**, where the walls are *"represented"* by the pointers of the cells that remained at NULL.  

This situation presents two problems:  
1. The use of the state of a cell (wall, corridor) completely loses its meaning in when all the cells are corridors.
2. The visual representation of this matrix is much more complex.

To solve these problems, generators such as *Prim* have been modified in order to carry out an excavation of **two** cells and **not more than one**, selecting cells according to the same rules of the generation algorithm. This modification leads to a labyrinth consisting of both wall and corridor cells. It also allows you to *"save"* considerably on the problem related to dead ends.

### Dead ends
Using an excavation technique of **two** cells instead of **one**, the creation of the labyrinth takes place with a completely reduced number of dead ends.  
This made it possible to avoid using the procedure for the elimination of dead ends.

The only recursive generator that takes advantage of the random elimination of dead ends is the one inspired by DFS.

So let's see the procedure for eliminating dead ends.

```c
if(muri==3){
    int nRand=0;
    int nMuri=F_genera_int_random(4);
    if(nMuri!=0) nRand=F_genera_int_random(4);

    switch (nMuri)
    {
        case 0: // 0 muro
            //F_elimina_muri(P,r,c,nMuri+3,nRand);
            break;
        case 1: // 1 muri
            F_elimina_muri(P,r,c,nMuri,nRand);
            break;
```
The procedure eliminates a number of chasubles of walls adjacent to the reference cell. As can be seen in the first condition to be verified, only the cells that are surrounded by *three* walls are considered. Once the cell is found, a random value between *zero* and *three* is generated which indicates the number of *n-walls* to be eliminated.  
The second randomly generated numerical value, nRand, indicates the starting position in which to check whether the cell has a wall or not. That is, assuming for example that an *nRand* value of three is generated, the procedure *F_eliminate_walls* will verify that the selected cell has a wall above it; an affirmative case eliminates it, otherwise it calls to test a new position. Instead if he had generated an nRand equal to one then he would have looked for a wall on the right side of the cell first.

If *nMuri* is zero, no wall is eliminated. It is possible, if desired, to enable the procedure even in the *zero* case, eliminating at least one wall at each cell considered dead end.

### L_Prim
```c
void F_prim_maze(Str P)
{
    Coda Q=NULL;
    M *initcell=F_cella_random(P);
    Q=F_enqueue(Q,initcell);
    P->nsize=0;
    F_aggiorna_nelem_coda(P,1);
    ...
    ...
    else{
        Q=F_dequeue(Q,cell);
        F_aggiorna_nelem_coda(P,0);
        }
}
```
As already described in *L_structure* section *Main structure*, the *Prim* algorithm uses the *nsize* value present in the main structure **P** as an integer indicating the number of total elements present in the queue.  
The *F_aggiorna_nelem_coda* function sends the value *one* in case the number of queue elements must be increased, otherwise *zero*.

Among the most interesting implementation details we certainly find the ease of searching for an *unvisited* cell, that is, a cell surrounded by four walls that the algorithm takes into consideration to carry out an excavation.  
Given the structure of a cell itself, it is not necessary to take its neighbors to check whether they are walls or not, simply check the value of its pointers to the weights. Just one of these is different from *NULL* to no longer make the cell *unviseted*.

The excavation procedure operates, as already speciﬁed in the analysis of the structures, by means of information relating to the position **p** of the cell.
```c
void F_dig_to(Str P, M *cell, M *newcell)
{
    Peso pes=F_alloca_struttura_peso();
    Peso pesmezz=F_alloca_struttura_peso();
    M *mezzo=NULL;

    switch (newcell->inf->p)
    {
        case 0: // DX
            mezzo=F_coord_nodo_dx(P,cell->inf->r,cell->inf->c);
            cell->pDX=pesmezz;
            mezzo->pSX=pesmezz;
            mezzo->pDX=pes;
            newcell->pSX=pes;

            break;
        ...
    }
}
```
Since the excavation takes place for two adjacent cells, two weights are created. Supposed to have cells **A**, **B** and **C**; one weight will be inserted between **A** and **B**, the second between **B** and **C**.  
Depending on the numerical value **p** contained in the destination cell, in our case it would be **C**, we proceed to insert the weights by taking the coordinates of the cell in the middle, in this case **B**.

The algorithm ends once the elements in the queue are finished.

### L_Dfs
Corresponds to the iterative DFS-inspired generator.  
The operation is completely *identical* to the **Prim** generator except for two details:

1. The support structure used for selecting the cells to operate on is a stack and not a *queue*.
2. The *stack* structure is used in its entirety. That is, when a cell must be removed from the structure, it is not randomly selected as in the Prim algorithm, but is **always** extracted from the top.

The analysis of the algorithm can be avoided as the excavation function *F_dig_to* is the same as that of the *Prim* algorithm, so please refer to the reading of this.

### L_Recursive
Represents the recursive type of DFS-inspired generator.  
Before executing the main algorithm, he proceeds to select a random cell in the labyrinth.
```c
void F_init_recursive_maze(Str P)
{
    M *cell=F_cella_random(P);
    int r=cell->inf->r;
    int c=cell->inf->c;
    free(cell->inf);
    cell->inf=NULL;

    F_recursive_maze(P,r,c);
}
```
A completely random labyrinth cell is selected which becomes the first corridor cell. Using the *inf* structure, the row and column of the chosen cell are taken and the generation algorithm is performed.
int *randDirs=F_direzioni_random();
```c
for(i=0;i<4;i++){
   switch (randDirs[i]){
       case 0: // DX
        if(F_dig_dx(P,r,c)) F_recursive_maze(P,r,c+2);
        break;
        ...
   }
   ...
}   
...
}
```
Random directions are selected in which to proceed with the excavation using the function called *F_direzioni_random*. This function creates an ordered integer vector, then mixes the values and returns them to the maze generation function.  
The algorithm then proceeds in a completely identical way to how it was structured.  
The particularity is certainly that of being similar to the algorithms seen before when a displacement and excavation of **two** cells is carried out.

### L_Divide
It differs significantly from other types of maze generation algorithms. This algorithm is of the recursive division generator type.  
As with *L_Recursive*, this algorithm, *L_Divide*, also proceeds first to an *"initialization"* phase and then calls the main algorithm.
```c
void F_init_divide(Str P)
{
    F_elimina_tutti_muri(P);
    F_divide(P,0,0,P->csize,P->rsize);
}
```
Being the only one that operates completely opposite to the others, it needs not to dig to introduce corridor cells but to have a matrix completely formed by corridors. This phase is obtained by the function *F_elimin_all_walls* which proceeds to connect each single cell with the adjacent cells.  
At the end of this phase, the division algorithm is called specifying the coordinates of the starting cell, in this case *zero-zero*, on which to randomize certain values and the size of the **room** on which to operate. At the beginning, the entire matrix is considered to be a single room passing precisely its number of columns and rows.  
According to the size of the room passed, it is divided into two parts, or cut by a horizontal wall, or vertical.

## L_Maze
It contains all the procedures used by the various labyrinth generators just described. Added to this are additional routines that manage the maze, such as its modification:
```c
mousemask(BUTTON1_CLICKED, NULL);
c=getch();

if(c==KEY_MOUSE)
{
    if(getmouse(&event)==OK)
    {
        if(event.bstate & BUTTON1_CLICKED)
        {
            int riga=event.y,colonna=event.x;
            ...
            F_sistema_archi_modifica_labirinto(P,riga-OFFSET_Y,colonna-OFFSET_X);
            ...
        }
        ...
    }
    ...
}
```
The procedure *F_modify_labyrinth* allows the user to be able to modify a cell of the labyrinth to transform it from wall to corridor and vice versa. It can be noted that in instruction *two* this change is made possible by enabling the use of the mouse by just clicking the left button. Any other keys will be ignored and the user will be notified on the screen. Supposed to have clicked on a cell of the labyrinth we proceed to its transformation. Obviously, the *OFFSET Y* and *OFFSET X* directives come into play, since being the representation of the matrix moved on the screen, the cell is actually selected by the user does not correspond to the exact row and column in the structural representation of the matrix.

The modification of a cell is completely trivial. We proceed to check if the cell has become a wall or a corridor, in the first case we proceed to the elimination of all the weights that the selected cell can have with the adjacent cells:
```c
if(matr[r][c].pDX!=NULL)
{
     matr[r][c+1].pSX=NULL;
     free(matr[r][c].pDX);
     matr[r][c].pDX=NULL;
}
```
If in this case there is a connection with the cell on its right, the pointer of the adjacent cell is set to *NULL*, deallocated the structure containing the weight by accessing from the initial cell, and subsequently also placed the pointer of the current cell at *NULL*.

Opposite case:
```c
if(cSX!=NULL)
{
      Peso pes=F_alloca_struttura_peso();
      cSX->pDX=pes;
      matr[r][c].pSX=pes;
}
```
the weight is allocated and the two cells are connected.

Other procedures are left to the developer's view from the source codes.

## Utility and management
## Generic libraries
Build the data structures, the types of generators of a maze, and given the search algorithms that operate on it; we go to the latter part of the documentation on the side relating to the entire management of the application and the functionality of the **curses** library.

### L_Utility
The *L_Utility* library is the one with the greatest number of functions and procedures used for user interaction and visual management of the labyrinth components and the structure of the whole program.

The initial procedure on which the application is based is:
```c
void F_start()
{
    F_avvia_curses();

    F_controllo_terminale();

    srand((unsigned)time(NULL));
 
    Str P=F_alloca_struttura_principale(); 
    
    F_crea_finestra_menu(P);
 
    F_stampa_menu_principale(P);
    
    F_dealloca_strutture(P); 

    F_termina_curses();
}
```
*F_start* is called from the *main* file. The procedure starts the curses mode and checks the terminal.  
The entire application has in fact been structured to be able to be executed and displayed with a terminal of standard size: 80x24. The terminal check procedure verifies that it is not smaller. If not, the user will be notified of the problem and urged to adapt the terminal to at least the pre-defined size.  
We move on to the allocation of the main structure and to that of the window reserved for the initial menu. Using the *F_stampa_menu_principale* procedure, you can print the first menu and wait for user input.

### Menu management
The menus are managed by a single structure.
```c
char *F_menu[]={
        "Genera labirinto",
        "Esci",

        "Prim",
        "DFS",
        "Recursive",
        "Divide",
        "INDIETRO",

        "Modifica labirinto",
        "Cerca percorso minimo",
        "Visualizza celle esplorate",
        "INDIETRO",
};
```
Depending on the visualization to be carried out, only certain choices are shown to the user. This management is given to the function *F_stampa_menu*:
```c
void F_stampa_menu(WINDOW *win,int selezionato,int partenza,int massimo,int x,int y)
{
    keypad(win,TRUE);
    refresh();

    int i;
    box(win,0,0);

    for(i=partenza;i<massimo;++i)
    {
        
        if(selezionato==i+1)
        {
            wattron(win,A_REVERSE);
            mvwprintw(win,y,x,"%s",F_menu[i]);
            wattroff(win,A_REVERSE);
        }
        else
        {
            mvwprintw(win,y,x,"%s",F_menu[i]);
        }
        ...
    }
...
}
```
which takes the following parameters as input:
 * *win*: pointer to the window to be displayed.
 * *selezionato*: indicates in terms of numerical value the selected element to which to apply a *"lighting"* effect (instruction *twelve*).
 * *partenza*: corresponds to the initial menu item. For example, if you have to print the second menu on the screen which shows the types of generators present, the starting value will be associated with the number *two-Prim*.
 * *massimo*: corresponds to the last element that can be displayed.
 * *x, y*: are the coordinates in which to go to print the menu item.
 
Departure and maximum are used not only as indexes for managing the menu to be printed, but also allow complete management of the entire menu shown on the screen. In fact, the user can start from the first element of the menu and go directly to the last and scroll upwards. Same thing if you find yourself selecting the last item, by scrolling down, you return to the first item in the menu.  
You can see how the menus are managed from the past *win* window. The box instruction creates an outline area with the default characters.
 
Once the menu is printed, its interaction with the user takes place through the *F_selected_menu* function which takes the same parameters as the previous procedure as input.
```c
while(1)
{
    int c=wgetch(win);

    switch(c)
    {
        case KEY_UP:
            if(selezionato==minimo) selezionato=massimo;
            else --selezionato;
        break;

        case KEY_DOWN:
            if(selezionato==massimo) selezionato=minimo;
            else ++selezionato;
        break;
        
        case 10:
            scelta=selezionato;
        break;
    }
   
    F_stampa_menu(win,selezionato,partenza,massimo,x,y);
    if(scelta!=0) break;
    
    }
...
}
```
The user can move through the menu only and exclusively using the directional arrows *KEY UP* and *KEY DOWN*. The *two* houses allow the management previously specified on being able to scroll the entire menu in an infinite loop (instructions *eight-nine* and *fourteen-fifteen*).  
Depending on the move, the menu is printed, calling the procedure described above, highlighting the chosen element.

The selection of the element is confirmed by pressing the *ENTER* key, corresponding to the numerical value associated with case *ten*. Then the numerical value of the choice is returned.

### Communication with the user
In addition to printing static information on which the user can inquire to decide the approach to be carried out, there are video prints that regulate the correct use of the application.

These are managed by a single procedure that performs three simple instructions:
```c
void F_stampa_pulisci_riga_labirinto(char *s)
{
    move(23,2);
    clrtoeol();
    mvprintw(23,2,"%s",s);
}
```
takes the string to be displayed as input, moves the cursor by means of a move to the selected row and column, and cleans the row and prints the new one. The row and column are pre-fixed as communication always takes place for a minimum support of an 80x24 terminal.

### Use of color
Color is used for the printing functionality of the cells explored by the A* algorithm. The procedure in charge is *F_celle_esplorate*:
```c
void F_celle_esplorate(Str P)
{
    int check=F_check_color();

    if(check)
    {
        start_color();
        init_pair(1,COLOR_WHITE,COLOR_RED);
        int x=use_default_colors();
        if(x!=0)
        {
            F_stampa_pulisci_riga_labirinto("Il terminale non supporta colori: ISO 6429. Premi invio per continuare.");
            getch();
            F_stampa_pulisci_riga_labirinto("Dopo potresti visualizzare colori divesi. Premi invio per continuare.");
            getch();
        }
        attron(COLOR_PAIR(1));
        F_visualizza_celle_esplorate(P);
        attroff(COLOR_PAIR(1));
    }
    else
    {
        F_visualizza_celle_esplorate(P);
    }
    ...
}
```
Support for the use of colors is verified through the *F_check_color* function which takes advantage of the predefined has_colors of the curses library. In the event of an answer in the affirmative, the use of colors is enabled and the ISO 6439 color support is verified, by means of an extension of the library, through the *use_default_colors*.  
Although most terminals use an ISO 6429 (ANSI) color sequence, in the event that this support is missing, the user will still be able to view the cells explored through the use of a color, but will be warned that the if subsequent interactions with the application may undergo a color change.

If the terminal does not support the use of colors, we will proceed in the same way to display the cells explored. This is possible because the procedure in charge of *F_visualizza_celle_esplorate* cells prints on screen a character that can be displayed both with a red background and without the use of color:
```c
addch (ACS_BULLET);
```

### Animations
The animation takes place after the printing of a minimum path obtained by one of the two search algorithms seen previously.
The procedure:
```c
void F_animazione_percorso(Str P,int r, int c, int rA, int cA)
```
it takes in input the main structure, the row and column of the starting cell and the arrival cell. The animation is a structure in the positioning of a character in the starting cell, and subsequently we proceed to search for those adjacent to that cell who have the visited state parts by *one*.
```c
addch('O');
refresh();

usleep(DELAY);

if(cellDx!=NULL && cellDx->visitato==1){  c=c+1; cellDx->visitato=0; }
        else
        {
            if(cellSx!=NULL && cellSx->visitato==1) {c=c-1; cellSx->visitato=0; 
        }
        ...
```
The character is then moved from one cell to another using the *"eﬀect"* created by the *usleep* procedure and at the same time the visit status of the cell is restored to its initial *zero* state. The animation ends once it reaches the arrival cell.

### Error handling
*"Testing a program can be used to show the presence of bugs, but never to show their absence." - Edsger Dijkstra*

The *L_Error* file allows you to notify the user in the event of *serious* errors that involve forced exit from the program.
```c
void F_error(int e)
{
    F_termina_curses();
    switch (e)
    {
        case 1:
            puts("\nError: errore di collegamento celle in: L_Prim - F_dig_to!\n");
            break;
        ...
exit(-1);
}      
```
The procedure takes as input a numerical value *"e"* corresponding to the type of error generated. We therefore proceed to close the curses mode and then move on to printing the error and forced *exit* via the system call exit.  
Many errors, despite being present, are irreproducible. That is, for correct programming, all instructions involving the use of a switch have a default case which, if not used, calls the error function. Unless substantial changes are made to the sources, since there are cycles with predefined values, the switches inside them will never fall in the *default* case.

# User Manual
## Overview
The following manual aims to facilitate the use of the application by means of a simple and quick overview of all the features available.

Before starting the program, the following requirements must be met:
 * Have keyboard and mouse. You can use the trackpad of your device as a pointing system as long as the use of the *left click* is performed by means of **physical** pressure.
 * Find yourself on a distribution based on linux kernel, or on an operating system with accessibility to a terminal that supports the use of the *curses* library.
 * Having installed a 'C' compiler and the appropriate *ncurses* library. Depending on your operating system, the installation operation may vary.
 * Terminal dimensions cannot be less than 80x24.

## Compilation and launch
Once the requirements necessary for the correct functioning of the application have been satisfied, it is compiled and executed.  
Once the archive has been unpacked, access the Source folder via the terminal.  
Then proceed with the compilation:
```c
gcc "all.c file" -o *
```
where in place of the asterisk '*', you can enter any name to associate with the executable once it has been completed.  
The program is then carried out using the command:
```c
./*
```
specifying the name previously used instead of an asterisk.

Se non si dispongono dei permessi necessari per l’esecuzione del programma, bisogna associare l’operazione di esecuzione tramite il comando:
```c
sudo chmod u+x *
```

## Interfaces
Once the executable has started, you will find yourself in the main screen. In each of the proposed screens, it is possible to move through the various options available by using the **directional arrows** on the keyboard and select the desired item using the **ENTER** key.
There are a total of three interfaces:


1. Main screen, is presented when the program starts. Two options are available:
 * *Genera labirinto*: a labyrinth generator system is selected from those proposed.
 * *ESCI*: termination of the program.
2. Maze generator selection screen. Four options are available:
 * *Prim*: generator inspired by PRIM.
 * *DFS*: generator inspired by DFS.
 * *Recursive*: recursive generator (inspired by DFS).
 * *Divide*: recursive division generator.
 * *INDIETRO*: you are returned to screen number *one*.
3. Maze management screen where you can perform certain operations on it, such as:
 * *Modifica labirinto*: allows modification of a labyrinth cell.
 * *Cerca percorso minimo*: search for the minimum path using one of the proposed algorithms.
 * *Visualizza celle esplorate*: video display of the cells explored by the search algorithm previously performed.
 * *INDIETRO*: you are returned to screen number *two*.

## Executable operations
The correct use of the operations that can be performed on the generated maze are shown in detail.  
The labyrinth is shown on the left of the menu for selecting the operation to be performed, and consists of two parts. The lighter part represents a wall, while the other represents a corridor.

You can **cancel** any operation by pressing any button on the keyboard, except those required to perform the chosen action.

### Modify maze
Allows you to change the status of a selected cell.  
If the starting cell represented a wall, its modification will make it a corridor, and the other way around.

Once the *modifica labirinto* option has been selected, the mouse pointer will be available. It is possible to move to any position of the labyrinth and by pressing with the **left mouse click**, the selected cell will be modified.


### Search for the minimum path
Once you have chosen this operation, you will be asked to select, using the mouse pointer, the cell of the labyrinth from which you wish to start.

The selection of the cell is done with the **left mouse click**.

If the selected cell is valid, the **P-artenza** character will be indicated visually at the chosen point. Once this phase is over, we move on to the selection of the arrival cell. In a totally equivalent way if the selected cell is valid, the **A**-rrivo character will be indicated at the chosen point.

In case of wrong choice, for example the selection of a cell that is a wall or the selection of any point outside the labyrinth, the search for the minimum path will be canceled.

Select the correct cells, you will be asked for the type of algorithm you want to use for finding the minimum path. Two options are available:
1. *Dijkstra*: algorithm for searching shortest paths.
2. *A**: heuristic algorithm for searching shortest paths.
It is possible to select one of the two algorithms by pressing on the keyboard the character **D** for the first type or **A** for the second, regardless of whether uppercase or lowercase.

If there is a minimum path, it will be displayed in the labyrinth by printing asterisk characters '*', otherwise you will be notified of the absence of a path.

If there is a minimum path, you are asked if you want to animate a character who follows this path.  
Two options are available:
1. *Si*: animation is carried out.
2. *Annulla*: non viene eﬀettuata alcuna animazione.

The selection can be made by **pressing** on the keyboard the character S for the first type or **ENTER** for the second, **regardless** of whether uppercase or lowercase.  
In the event of an erm ermative response, the animation operation will be performed and any other operation proposed from the menu can be chosen subsequently.

### Viewing cells explored
It allows you to view the cells and is effectively explored by the search algorithm A*.

More information on why this option is not available for Dijkstra's algorithm can be found in the Developer Manual section *L_Structure*, subsection Main structure.  
Once the search for the minimum path using A * has been performed, you are returned to the labyrinth management menu. From here you can select the option display cells explored. Once you hit enter, all the cells explored by the algorithm will be displayed on the screen, through red colored cells with white dots in the center.

In the event that the colored cells are not visible, but only the dot-shaped characters are present, unfortunately, the terminal used does not support the use of colors. You will obviously be warned if this condition occurs.

Once the explored cells have been displayed, once you press enter, the labyrinth will be cleaned, and you will return to the management menu.
