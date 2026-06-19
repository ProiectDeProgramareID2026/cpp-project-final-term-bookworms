#include "CliApplication.h"

#include "Book.h"

#include <iostream>
#include <string>

int CliApplication::run(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    return dispatch(argc, argv);
}

void CliApplication::printUsage(std::ostream& out) {
    out
        << "Usage:\n"
        << "  library -help\n"
        << "  library list\n"
        << "  library add-book \"<title>\" \"<author>\" \"<type>\" \"<location>\"\n"
        << "  library search-title \"<query>\"\n"
        << "  library search-author \"<query>\"\n"
        << "  library search-available\n"
        << "  library search-borrowed\n"
        << "  library borrow <id>\n"
        << "  library return <id>\n";
}

bool CliApplication::parsePositiveId(const char* arg, int& outId) {
    if (arg == nullptr || *arg == '\0') {
        return false;
    }

    try {
        const long value = std::stol(arg);
        if (value <= 0 || value > 2147483647L) {
            return false;
        }
        outId = static_cast<int>(value);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool CliApplication::validateAddBookArgs(const std::string& title, const std::string& author,
                                         const std::string& type, const std::string& location) {
    if (!Book::isValidField(title)) {
        std::cerr << "Invalid title: must be non-empty and must not contain '|'.\n";
        return false;
    }
    if (!Book::isValidField(author)) {
        std::cerr << "Invalid author: must be non-empty and must not contain '|'.\n";
        return false;
    }
    if (!Book::isValidField(type)) {
        std::cerr << "Invalid type: must be non-empty and must not contain '|'.\n";
        return false;
    }
    if (!Book::isValidField(location)) {
        std::cerr << "Invalid location: must be non-empty and must not contain '|'.\n";
        return false;
    }
    return true;
}

int CliApplication::dispatch(int argc, char* argv[]) {
    const std::string command = argv[1];

    if (command == "-help" || command == "--help" || command == "help") {
        if (argc != 2) {
            std::cerr << "Usage: library -help\n";
            return 1;
        }
        printUsage(std::cout);
        return 0;
    }

    if (command == "list") {
        if (argc != 2) {
            std::cerr << "Usage: library list\n";
            return 1;
        }
        library_.displayBooks();
        return 0;
    }

    if (command == "add-book") {
        if (argc != 6) {
            std::cerr << "Usage: library add-book \"<title>\" \"<author>\" \"<type>\" \"<location>\"\n";
            return 1;
        }

        const std::string title = argv[2];
        const std::string author = argv[3];
        const std::string type = argv[4];
        const std::string location = argv[5];

        if (!validateAddBookArgs(title, author, type, location)) {
            return 1;
        }

        if (!library_.addBook(title, author, type, location)) {
            std::cerr << "Failed to add book.\n";
            return 1;
        }

        std::cout << "Book added successfully.\n";
        return 0;
    }

    if (command == "search-title") {
        if (argc != 3) {
            std::cerr << "Usage: library search-title \"<query>\"\n";
            return 1;
        }
        const std::string query = argv[2];
        if (query.empty()) {
            std::cerr << "Search query cannot be empty.\n";
            return 1;
        }
        library_.searchByTitle(query);
        return 0;
    }

    if (command == "search-author") {
        if (argc != 3) {
            std::cerr << "Usage: library search-author \"<query>\"\n";
            return 1;
        }
        const std::string query = argv[2];
        if (query.empty()) {
            std::cerr << "Search query cannot be empty.\n";
            return 1;
        }
        library_.searchByAuthor(query);
        return 0;
    }

    if (command == "search-available") {
        if (argc != 2) {
            std::cerr << "Usage: library search-available\n";
            return 1;
        }
        library_.displayAvailable();
        return 0;
    }

    if (command == "search-borrowed") {
        if (argc != 2) {
            std::cerr << "Usage: library search-borrowed\n";
            return 1;
        }
        library_.displayBorrowed();
        return 0;
    }

    if (command == "borrow") {
        if (argc != 3) {
            std::cerr << "Usage: library borrow <id>\n";
            return 1;
        }

        int id = 0;
        if (!parsePositiveId(argv[2], id)) {
            std::cerr << "Invalid book ID: expected a positive integer.\n";
            return 1;
        }

        return library_.borrowBook(id) ? 0 : 1;
    }

    if (command == "return") {
        if (argc != 3) {
            std::cerr << "Usage: library return <id>\n";
            return 1;
        }

        int id = 0;
        if (!parsePositiveId(argv[2], id)) {
            std::cerr << "Invalid book ID: expected a positive integer.\n";
            return 1;
        }

        return library_.returnBook(id) ? 0 : 1;
    }

    std::cerr << "Unknown command: " << command << '\n';
    printUsage();
    return 1;
}
