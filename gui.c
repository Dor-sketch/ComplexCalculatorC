#include "complex.h"
#include <ctype.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 15

// Function declarations
static void activate(GtkApplication *app, gpointer user_data);
void on_button_clicked(GtkWidget *widget, gpointer data);

// Global Widgets
GtkWidget *input_entry;
GtkWidget *output_text;
GtkWidget *output_label;
GtkTextBuffer *buffer;
GtkTextIter iter;
char output[MAX];



int output_index = 0;
// Function to update the text entry with the button label
void update_input_with_button(GtkWidget *widget, gpointer data) {
    puts("update_input_with_button - WIP");
}

// Activate function: Sets up the GTK window and widgets
void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *box, *buttons_box, *num_box, *op_box;
    GtkWidget *instructions;
    const char *comps[] = {"A", "B", "C", "D", "E", "F"};
    const char *num_labels[] = {"0", "1", "2", "3", "4",
                                "5", "6", "7", "8", "9"};
    const char *op_labels[] = {"+",
                               "-",
                               "*",
                               "/",
                               "abs_comp",
                               "add_comp",
                               "sub_comp",
                               "mult_comp_real",
                               "mult_comp_img",
                               "mult_comp_comp"};
    GtkWidget *button;

    // Create a new window and set its title
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Complex Calculator");

    // Construct the main container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create instructions label
    instructions =
        gtk_label_new("Welcome to the Complex Calculator!\n"
                      "To use this calculator, type in a command and press "
                      "enter.\n"
                      "The following commands are available:\n"
                      "   - abs_comp A // print |A|\n"
                      "   - add_comp A B // print A+B\n"
                      "   - sub_comp A B // print A-B\n"
                      "   - mult_comp_real A B // print A*B\n"
                      "   - mult_comp_img A B // print A*B\n"
                      "   - mult_comp_comp A B // print A*B\n"
                      "   - stop // stop the program\n\n"
                      "To stop the program, type 'stop'.\n"
                      "------------------------------------------------\n");
    gtk_box_pack_start(GTK_BOX(box), instructions, TRUE, TRUE, 0);
    // Create a grid for all buttons
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(box), grid);

    // Create number buttons and add them to the grid
    for (int i = 0; i < 10; i++) {
        GtkWidget *button = gtk_button_new_with_label(num_labels[i]);
        g_signal_connect(button, "clicked",
                         G_CALLBACK(update_input_with_button), entry);

        // Calculate the row and column numbers
        int row = 4 - i / 3; // 4 rows for numbers 1-9 and 0 at the bottom
        int col = i % 3;     // 3 columns for numbers

        // Add the button to the grid
        gtk_grid_attach(GTK_GRID(grid), button, col+1, row, 1, 1);
    }

    // Create hexadecimal digit buttons and add them to the grid
    for (int i = 0; i < sizeof(comps) / sizeof(comps[0]); i++) {
        GtkWidget *button = gtk_button_new_with_label(comps[i]);
        g_signal_connect(button, "clicked",
                         G_CALLBACK(update_input_with_button), entry);

        // Calculate the row and column numbers
        // 2 rows for hex digits, next to the number buttons
        int row = i / 2 + 1;
        int col =
            i % 2 + 2; // 3 columns for hex digits, next to the number buttons

        // Add the button to the grid
        gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
    }

    // Create operation buttons and add them to the grid
    for (int i = 0; i < sizeof(op_labels) / sizeof(op_labels[0]); i++) {
        GtkWidget *button = gtk_button_new_with_label(op_labels[i]);
        g_signal_connect(button, "clicked",
                         G_CALLBACK(update_input_with_button), entry);

        // Calculate the row and column numbers
        int row = i / 2; // 2 rows for operation buttons
        int col =
            i % 2 +
            5; // 2 columns for operation buttons, next to the hex digit buttons

        // Add the button to the grid
        gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
    }

    // Create a new text entry field
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, TRUE, TRUE, 0);

    // Create a new text view
    output_text = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(box), output_text, TRUE, TRUE, 0);

    // Create a new text buffer and add it to the text view
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_text));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    gtk_text_buffer_insert(buffer, &iter, "Output:\n", -1);

    button = gtk_button_new_with_label("Enter");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked),
    entry);


    // Show all widgets within the window
    gtk_widget_show_all(window);
}

void on_button_clicked(GtkWidget *widget, gpointer data) {

        int pipefd_in[2], pipefd_out[2];
        pipe(pipefd_in);
        pipe(pipefd_out);

        pid_t pid = fork();

        if (pid == 0) {          // Child process
            close(pipefd_in[1]); // Close the write end of the input pipe
            dup2(pipefd_in[0], STDIN_FILENO); // Redirect stdin
            close(
                pipefd_in[0]); // Close the original read end of the input pipe

            close(pipefd_out[0]); // Close the read end of the output pipe
            dup2(pipefd_out[1], STDOUT_FILENO); // Redirect stdout
            close(pipefd_out[1]); // Close the original write end of the output
                                  // pipe

            // Execute commands
            run_commands();

            exit(EXIT_FAILURE);
        } else {                  // Parent process
            close(pipefd_in[0]);  // Close the read end of the input pipe
            close(pipefd_out[1]); // Close the write end of the output pipe
        }

        // Get input and write to pipe
        const char *input = gtk_entry_get_text(GTK_ENTRY(data));
        strcat(input, "\n");
        write(pipefd_in[1], input, strlen(input));
        close(pipefd_in[1]); // Close the write end of the input pipe after
                             // writing

        // read from pipe
        char output[MAX];
        int bytes_read = read(pipefd_out[0], output, MAX);

        printf("bytes read: %d\n", bytes_read);
        printf("output: %s\n", output);

        // Clear the input entry field
        gtk_entry_set_text(GTK_ENTRY(data), output);
}


        // No need to wait for the child process here, as it should keep running
        // Main function
        int main(int argc, char **argv) {
            printf("Starting GUI...\n");
            GtkApplication *app;
            int status;

            app = gtk_application_new("my.comp2", G_APPLICATION_FLAGS_NONE);
            g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
            status = g_application_run(G_APPLICATION(app), argc, argv);
            g_object_unref(app);

            return status;
}
