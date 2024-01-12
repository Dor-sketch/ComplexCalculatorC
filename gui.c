#include "complex.h"
#include <ctype.h>
#include <gtk/gtk.h>
#include <unistd.h>

// Function declarations
void on_button_clicked(GtkWidget *widget, gpointer data);

// Activate function: Sets up the GTK window and widgets
void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *entry;
    GtkWidget *box;

    // Create a new window, and set its title
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Complex Calculator");

    // Construct the container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create a new text entry field
    entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(box), entry);

    // Create a new button
    button = gtk_button_new_with_label("Enter");
    gtk_container_add(GTK_CONTAINER(box), button);

    // Connect the button's "clicked" signal to the on_button_clicked function
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entry);

    // Show all widgets within the window
    gtk_widget_show_all(window);
}
void on_button_clicked(GtkWidget *widget, gpointer data) {
    static int pipefd[2] = {-1, -1};
    static pid_t pid = -1;

    if (pipefd[0] == -1 && pipefd[1] == -1) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {                    // Child process
            close(pipefd[1]);              // Close the write end of the pipe
            dup2(pipefd[0], STDIN_FILENO); // Redirect stdin
            close(pipefd[0]);              // Close the original read end

            // Execute commands
            run_commands();
            exit(EXIT_FAILURE);
        } else {              // Parent process
            close(pipefd[0]); // Close the read end of the pipe
        }
    }

    char *input = gtk_entry_get_text(GTK_ENTRY(data));

    // add a newline to the end of the input
    input = realloc(input, strlen(input) + 2);
    input[strlen(input)] = '\n';

    if (write(pipefd[1], input, strlen(input)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Clear the input entry field
    gtk_entry_set_text(GTK_ENTRY(data), "");

    // No need to wait for the child process here, as it should keep running
}

// Main function
int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("my.calc", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    if (status = g_application_run(G_APPLICATION(app), argc, argv))
        perror("g_application_run");
    g_object_unref(app);

    return status;
}
