# ComplexCalculatorC 🧮

ComplexCalculatorC is a C program that allows you to perform complex number calculations and explore mathematical concepts in the C programming language. It provides a set of commands to work with complex numbers, making it a useful tool for mathematical calculations and experimentation.

This program was created as a homework assignment for the _System programming Laboratory course (20465)_ at the Open University of Israel, and earned a score of `98`.

![Alt text](images/image-1.png)


_**Update**: This repository now includes a new expression parser and evaluator written in C: [calc.c](calc.c). This program parses mathematical expressions and evaluates them using an expression tree. It can be used as a standalone program or as a library for other projects. See the [Expression Parser and Evaluator](#expression-parser-and-evaluator-) section for more details._


## Features ✨

- **Complex Number Operations:** ComplexCalculatorC supports various complex number operations, including addition, subtraction, multiplication, absolute value calculations, and more.

- **User-Friendly Interface:** The program offers a user-friendly command-line interface, making it easy to input complex numbers and perform calculations.

- **Multiple Variables:** You can work with up to six complex variables (A, B, C, D, E, F) simultaneously, allowing for complex calculations involving multiple variables.

## Usage 🚀

1. **Building the Program:**

   To compile the program, use the following command:

   ```bash
   make
    ```

## New GUI! 🖥️

ComplexCalculatorC now includes a graphical user interface (GUI) for a more intuitive user experience. The GUI is built using the `Gtk+` library and allows you to perform complex number calculations and explore mathematical concepts in a visual environment.

![Alt text](images/image-2.png)

## Commands ✍️

ComplexCalculatorC supports a variety of commands for complex number operations, enhancing mathematical exploration:

| Command           | Syntax                                           | Description |
|-------------------|--------------------------------------------------|-------------|
| `print_comp`      | `print_comp <variable>`                          | Prints the specified complex variable. |
| `abs_comp`        | `abs_comp <variable>`                            | Calculates and displays the absolute value of a complex variable. |
| `mult_comp_real`  | `mult_comp_real <variable>, <real_number>`        | Multiplies a complex variable by a real number and displays the result. |
| `mult_comp_img`   | `mult_comp_img <variable>, <imaginary_number>`    | Multiplies a complex variable by an imaginary number. |
| `read_comp`       | `read_comp <variable>, <real_part>, <imaginary_part>` | Assigns real and imaginary parts to a complex variable. |
| `add_comp`        | `add_comp <variable1>, <variable2>`              | Adds two complex variables and prints the outcome. |
| `sub_comp`        | `sub_comp <variable1>, <variable2>`              | Subtracts the second complex variable from the first and displays the result. |
| `mult_comp_comp`  | `mult_comp_comp <variable1>, <variable2>`        | Multiplies two complex variables and presents the result. |
| `stop`            | `stop`                                          | Exits the program. |

Each command is designed to be intuitive and user-friendly, catering to both beginners and experienced users interested in complex number computations.

## Testing 🧪

To facilitate testing of ComplexCalculatorC, two text files are provided:

- [input.txt](input.txt): Contains a series of valid inputs for testing various functionalities of the program. You can use these inputs to see how the program handles typical use cases.

    ![Alt text](images/correct_input.png)
    _Example of valid inputs in `input.txt`_

- [invalid_input.txt](invalid_input.txt): Includes examples of invalid inputs to test the program's error handling and robustness. This can help ensure that the program gracefully handles unexpected or incorrect user input.

    ![Alt text](images/invalid_input.png)
    _Example of invalid inputs in `invalid_input.txt`_

Feel free to use these files to experiment with the program and understand its capabilities and limitations.

## Contributing 🤝

Contributions to ComplexCalculatorC are welcome! If you have suggestions for improvement or have identified issues, please feel free to open an issue or submit a pull request on the [GitHub repository](https://github.com/Dor-sketch/ComplexCalculatorC).

## License 📜

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

🚀 Happy Exploring in the World of Complex Numbers! 🧮

---

## New: Expression Parser and Evaluator 🧮

This project is a C-based expression parser and evaluator. It constructs an expression tree from mathematical expressions and then evaluates them. Perfect for learning and understanding how parsing works in compilers!

The program was originaly designed for the leetcode problem [Basic Calculator](https://leetcode.com/problems/basic-calculator/), but it can be used as a standalone program or as a library for other projects.

### Features 🌟

- **Expression Parsing**: Converts a string representation of a mathematical expression into an expression tree.
- **Unary and Binary Operations**: Handles both unary (e.g., `-5`) and binary (e.g., `5 + 3`) operations.
- **Nested Expressions**: Supports expressions within parentheses (e.g., `(3 + (2 - 1))`).
- **Inline Functions**: Utilizes inline functions for efficient calculation.
- **Readable Tree Output**: Prints the structure of the expression tree for easy visualization.

### How to Compile 🛠️

To compile the program, use the same makefile as before:

```bash
make
```

This command compiles the `calc.c` file using the C99 standard with level 2 optimizations, which includes function inlining.

### Usage 📖

Run the compiled program from the command line. You can input mathematical expressions, and the program will parse these expressions, construct an expression tree, and then evaluate them.

Simply execute `./expression_parser` and enter your expression at the prompt. The program will output the evaluated result along with a visual representation of the expression tree.

### Example

The program will parse the expression, construct the tree, and output the result along with the tree structure:

Output of [calc_tests.txt](calc_tests.txt):

![Alt text](calc_example.png)

### Contributing 🤝

Contributions to this project are welcome! Whether it's bug fixes, improvements, or new feature suggestions, feel free to fork the repository and submit a pull request.

### License 📜

This project is open-source and available under the [MIT License](LICENSE).

### Acknowledgements 👏

Special thanks to all contributors and users who provided valuable feedback and suggestions for this project.

### Contact 📧

Feel free to reach out for any questions or discussions related to this project. Contact information can be found in my GitHub profile.

---

Happy coding! 😄
