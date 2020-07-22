# Green_vs_Red
Game of life styled program with a CLI UI with focus on OOP paradigms. Built using g++ 9.3.0 on 64-bit Linux.

# Structure
The main advantage of this program over other variants of Conway's Game of Life is the ability to add more Cell states and their own logic added to them 
than the original dead or alive states. 

  1. Cells are governed by their board that has ownership over them. Each cell has their own coordinates, shape/color and what the future type of Cell 
  they'll have to turn into after the generation ends. To add a cell type, you'd need to derive from the superclass - Cell and from there you'd need only to define the 
          constructor (with coordinates and shape/color), and the pure virtual function `nextGen`.
      * Default cell rules:
        - Green (`1`) - **won't** turn `Red (0)` if it is surrounded by exactly 2, 3 or 6 `Green (1)` cells.
        - Red (`0`) - **will** turn `Green (1)` if it is surrounded by exactly 3 or 6 `Green (1)` cells.
      * Available member functions:
        - Pure virtual `nextGen(board)` - define what kind of logic the cell should follow and what it's type should become whenever the end of the generation 
        comes. To do that, move a unique pointer to the `futureCell`.
        - Virtual default destructor - in case a future derived class needs to have their own explicit constructor.
        - `getFutureCell` - returns the location of the future cell that's to be assigned in the next cycle.
  
  2. A board contains all of the cells in a two-dimentional vector full of unique pointers, the height and width of the vector, and the number of generations that
  have already passed.
      * Available member functions:
        - `print()` and `operator<<` - prints to the console the whole two-dimentional array with the assigned cell shape/color. Cells are seperated by tabs and new lines.
        Prints a `NULL` or `.` character when the current location in the vector points to a `nullptr`.
        - `clear()` - resets all of the pointed locations memory and makes all elements in the two-dimentional vector point to `nullptr`.
        - `update()` - initializes a single generation. Firstly, it goes through all of the cells in the vector and calls the `nextGen` function of all of the
        cells sequentially. They generate their future cell type and after crawling through the whole array, all of the future types are applied to their original
        cell's caller and location, and the previous object is destroyed. If the `futureCell` pointer points to a `nullptr`, the cell stays the same. A generation
        accounted at the end of the function.
        - `getGens()` - returns the number of generations that have passed until this point.
        - `getCellShapeAt(y,x)` - returns the shape/color (character) at the given coordinates in the two-dimentional vector.
        - `getCellArea(Cell)` - returns a structure cointaining the coordinates to form a boxed area in the vector. That area is where a cell could search its
        surroundings like the default ones.
        - `getSize()` - return the height and width of the current board.
        - `addCell(Cell)` - will move the inputted cell into the stored coordinates. If the cell's coordinates do not comform to the board height and width it gets
        ignored. Similarly, if the inputted cell points to a `nullptr` or the position on the board the cell's coordinates pointes to an already occupied position
        the inputted cell gets ignored.
        - `changeCell(Cell)` - similar to `addCell(Cell)` but overwrites the cell at the inputed cell's coordinates with it.
        - `fillEmptyWith<Cell>()` - fills the remaining free slots (pointers to `nullptr`) with the given type. Only accepts derivatives of `Cell`.
        - `fillFromConsole()` and `operator>>` - allows for the manual allocation of cell types while typing in the color/shapes of the cells. Newly defined cells
        need to have a specific case for them in the switch statement.
        - `operator<<` and `operator>>` are acompanied by an extractor(`ext(os)`) and inserter (`ins(is)`). They allow for the native handling of the variables
        in the class without the need of elevated privaledges on external functions.
  3. Enum containing all default characters and an associated human readable name. Adding new shapes/colors here is *recommended*.
  4. The program is restricted to maximum size of **999x999** of the board and coordinates of the cells.zz
  5. The UI is contained in the main file. The UI consists of barebones input where functions from `CellBoard` are used such as `fillfromConsole()` and `update()` to
  have basic functionality. Main way to gather input from the user is with the previously mentioned functions and a function in main that defines a getline that strips
  out all of the unneeded characters that are other than numbers. For a number to be identified between others, it has to be spaced with at least *some* other character.
    
