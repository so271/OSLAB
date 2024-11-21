#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <sys/wait.h>
#include <utime.h>
#include <cerrno>
#include <sys/utsname.h>
#include <ctime>
#include <dirent.h>

// Function prototypes
void processRelatedSystemCalls();
void fileRelatedSystemCalls();
void communicationSystemCalls();
void informationRelatedSystemCalls();
void advancedFileOperations();

int main() {
    int choice;
    while (true) {
        std::cout << "\nSystem Call Demonstration\n";
        std::cout << "1. Process Related System Calls (fork, exit, wait)\n";
        std::cout << "2. File Related System Calls (open, read, write, close, link, unlink, stat)\n";
        std::cout << "3. Communication Related System Calls (pipe, fifo)\n";
        std::cout << "4. Information Related System Calls\n";
        std::cout << "5. Advanced File Operations (Directory manipulation, timestamp)\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                processRelatedSystemCalls();
                break;
            case 2:
                fileRelatedSystemCalls();
                break;
            case 3:
                communicationSystemCalls();
                break;
            case 4:
                informationRelatedSystemCalls();
                break;
            case 5:
                advancedFileOperations();
                break;
            case 6:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

// 1. Process Related System Calls: fork, exit, wait, exec
void processRelatedSystemCalls() {
    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        perror("Fork failed");
        return;
    } else if (pid == 0) { // Child process
        std::cout << "Child process (PID = " << getpid() << ") created. Executing `ls` command.\n";
        execlp("ls", "ls", "-l", nullptr);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else { // Parent process
        int status;
        wait(&status); // Wait for child process to complete
        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status: " << WEXITSTATUS(status) << '\n';
        }
        std::cout << "Parent process (PID = " << getpid() << ") continues.\n";
    }
}

// 2. File Related System Calls: open, read, write, close, stat
void fileRelatedSystemCalls() {
    int fd = open("testfile.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
    if (fd < 0) {
        perror("File open failed");
        return;
    }

    // Write to the file
    const char *data = "System call demonstration in C++.\n";
    write(fd, data, strlen(data));

    // Read from the file
    char buffer[100];
    lseek(fd, 0, SEEK_SET); // Reset file offset to beginning
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        perror("File read failed");
    } else {
        buffer[bytesRead] = '\0';
        std::cout << "Data read from file: " << buffer;
    }

    // Get file metadata using stat
    struct stat fileStat;
    if (stat("testfile.txt", &fileStat) == 0) {
        std::cout << "\nFile: 'testfile.txt'\n";
        std::cout << "File size: " << fileStat.st_size << " bytes\n";
        std::cout << "Last modified: " << ctime(&fileStat.st_mtime);
        std::cout << "File permissions: " << (fileStat.st_mode & 0777) << " (octal)\n";
    } else {
        perror("Stat failed");
    }

    close(fd); // Close the file
}

// 3. Communication Related System Calls: pipe, FIFO
void communicationSystemCalls() {
    int pipefd[2];
    char buffer[100];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return;
    }

    pid = fork(); // Create a child process
    if (pid < 0) {
        perror("Fork failed");
        return;
    } else if (pid == 0) { // Child process
        close(pipefd[0]); // Close reading end
        const char *message = "Message from child to parent through pipe.\n";
        write(pipefd[1], message, strlen(message));
        close(pipefd[1]); // Close writing end
        exit(0);
    } else { // Parent process
        close(pipefd[1]); // Close writing end
        read(pipefd[0], buffer, sizeof(buffer) - 1);
        std::cout << "Parent received: " << buffer;
        close(pipefd[0]); // Close reading end
        wait(nullptr); // Wait for child
    }
}

// 4. Information Related System Calls: uname, getpid, getppid, etc.
void informationRelatedSystemCalls() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    uid_t uid = getuid();
    gid_t gid = getgid();

    std::cout << "Current PID: " << pid << ", Parent PID: " << ppid << '\n';
    std::cout << "User ID: " << uid << ", Group ID: " << gid << '\n';

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << '\n';
    } else {
        perror("getcwd failed");
    }

    struct utsname sysInfo;
    if (uname(&sysInfo) == 0) {
        std::cout << "System Information:\n";
        std::cout << "  System name: " << sysInfo.sysname << '\n';
        std::cout << "  Node name: " << sysInfo.nodename << '\n';
        std::cout << "  Release: " << sysInfo.release << '\n';
        std::cout << "  Version: " << sysInfo.version << '\n';
        std::cout << "  Machine: " << sysInfo.machine << '\n';
    } else {
        perror("uname failed");
    }
}

// 5. Advanced File Operations: Directory manipulation, timestamp
void advancedFileOperations() {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(".")) == nullptr) {
        perror("opendir failed");
        return;
    }

    std::cout << "Files in current directory:\n";
    while ((entry = readdir(dir)) != nullptr) {
        std::cout << "  " << entry->d_name << '\n';
    }
    closedir(dir);

    // Change file modification time
    struct utimbuf new_times;
    new_times.actime = time(nullptr);
    new_times.modtime = time(nullptr);

    if (utime("testfile.txt", &new_times) == 0) {
        std::cout << "Modification time updated for 'testfile.txt'\n";
    } else {
        perror("utime failed");
    }
}
