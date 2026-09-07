#include <iostream>
#include <bits/this_thread_sleep.h>

#include "libs/DrawerIO.h"
#include "libs/StringUtils.h"

void PrintHelp() {
    std::cout << "╔═══════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                         DRAWER CLI v1.0                                   ║" << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Available commands:                                                      ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  AddShape <id> <color> <type> <params...>                                 ║" << std::endl;
    std::cout << "║      Add a new shape to the picture                                       ║" << std::endl;
    std::cout << "║      <id>     - Unique identifier for the shape                           ║" << std::endl;
    std::cout << "║      <color>  - Color in #RRGGBB format                                   ║" << std::endl;
    std::cout << "║      <type>   - Shape type: circle, rectangle, triangle, line, text       ║" << std::endl;
    std::cout << "║      <params> - Shape-specific parameters                                 ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  MoveShape <id> <dx> <dy>                                                 ║" << std::endl;
    std::cout << "║      Move a shape by vector (dx, dy)                                      ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  MovePicture <dx> <dy>                                                    ║" << std::endl;
    std::cout << "║      Move all shapes by vector (dx, dy)                                   ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  DeleteShape <id>                                                         ║" << std::endl;
    std::cout << "║      Delete a shape from the picture                                      ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  List                                                                     ║" << std::endl;
    std::cout << "║      List all shapes in the picture                                       ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  ChangeColor <id> <color>                                                 ║" << std::endl;
    std::cout << "║      Change the color of a shape                                          ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  ChangeShape <id> <type> <params...>                                      ║" << std::endl;
    std::cout << "║      Change the geometry of a shape                                       ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  DrawShape <id>                                                           ║" << std::endl;
    std::cout << "║      Draw a single shape on the canvas                                    ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  DrawPicture                                                              ║" << std::endl;
    std::cout << "║      Draw all shapes on the canvas                                        ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  Save <format> <filename>                                                 ║" << std::endl;
    std::cout << "║      Save the picture to a file                                           ║" << std::endl;
    std::cout << "║      <format>  - svg or png                                               ║" << std::endl;
    std::cout << "║      <filename> - Path to the output file                                 ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  help                                                                     ║" << std::endl;
    std::cout << "║      Show this help message                                               ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  exit / quit                                                              ║" << std::endl;
    std::cout << "║      Exit the program                                                     ║" << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Shape types and their parameters:                                        ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  circle <x> <y> <r>                                                       ║" << std::endl;
    std::cout << "║      Circle with center at (x, y) and radius r                            ║" << std::endl;
    std::cout << "║      Example: AddShape c1 #FF0000 circle 100 200 25                       ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  rectangle <left> <top> <width> <height>                                  ║" << std::endl;
    std::cout << "║      Rectangle with top-left corner at (left, top)                        ║" << std::endl;
    std::cout << "║      Example: AddShape r1 #00FF00 rectangle 10 20 30 40                   ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  triangle <x1> <y1> <x2> <y2> <x3> <y3>                                   ║" << std::endl;
    std::cout << "║      Triangle with vertices at (x1,y1), (x2,y2), (x3,y3)                  ║" << std::endl;
    std::cout << "║      Example: AddShape t1 #0000FF triangle 0 0 10 0 0 10                  ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  line <x1> <y1> <x2> <y2>                                                 ║" << std::endl;
    std::cout << "║      Line segment from (x1,y1) to (x2,y2)                                 ║" << std::endl;
    std::cout << "║      Example: AddShape l1 #FFFFFF line 10 20 35 88                        ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  text <left> <top> <fontSize> <text...>                                   ║" << std::endl;
    std::cout << "║      Text with top-left corner at (left, top) and font size               ║" << std::endl;
    std::cout << "║      Example: AddShape t1 #FFAA88 text 100.3 100.2 12.8 Hello world       ║" << std::endl;
    std::cout << "║                                                                           ║" << std::endl;
    std::cout << "║  Examples:                                                                ║" << std::endl;
    std::cout << "║    AddShape sh1 #FF00FF circle 100 110 15                                 ║" << std::endl;
    std::cout << "║    MoveShape sh1 10.5 -5.2                                                ║" << std::endl;
    std::cout << "║    ChangeColor sh1 #00FF00                                                ║" << std::endl;
    std::cout << "║    ChangeShape sh1 rectangle 0 0 50 30                                    ║" << std::endl;
    std::cout << "║    DrawShape sh1                                                          ║" << std::endl;
    std::cout << "║    Save svg output.svg                                                    ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════════════════╝" << std::endl;
}

int main() {
    DrawerIO drawer;
    std::string line;

    std::cout << "Drawer CLI v1.0" << std::endl;
    std::cout << "Type 'help' for commands, 'exit' to quit" << std::endl;
    std::cout << "> ";

    while (std::getline(std::cin, line)) {
        Trim(line);

        if (line == "exit") {
            break;
        }

        if (line == "help") {
            PrintHelp();
            std::cout << "> ";
            continue;
        }

        auto tokens = Separate(line, ' ');
        drawer.HandleInput(tokens);
        std::cout << "> ";
    }
}
