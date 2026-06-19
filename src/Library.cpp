    #include "Library.h"

    #include <algorithm>
    #include <cctype>
    #include <iostream>

    namespace {
    // Case-insensitive substring matching by converting both strings to lowercase before comparison
    bool containsCaseInsensitive(const std::string& haystack, const std::string& needle) {
        if (needle.empty()) {
            return false;
        }

        std::string hayLower = haystack;
        std::string needleLower = needle;
        std::transform(hayLower.begin(), hayLower.end(), hayLower.begin(),
                    [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        // Convert both strings to lowercase to allow case-insensitive search.
        std::transform(needleLower.begin(), needleLower.end(), needleLower.begin(),
                    [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        return hayLower.find(needleLower) != std::string::npos;
    }
    }  // namespace

    Library::Library() {
        loadBooks();
    }

    void Library::loadBooks() {
        books_ = fileManager_.readBooksFromFile();
    }

    void Library::saveBooks() {
        fileManager_.writeBooksToFile(books_);
    }
    // Validates input data, generates a unique ID and stores the new book in memory and on disk.
    bool Library::addBook(const std::string& title, const std::string& author,
                        const std::string& type, const std::string& location) {
        if (!Book::isValidField(title) || !Book::isValidField(author) ||
            !Book::isValidField(type) || !Book::isValidField(location)) {
            return false;
        }

        const int id = nextId();
        books_.push_back(std::make_unique<Book>(id, title, author, type, location));
        saveBooks();
        return true;
    }

    void Library::searchByTitle(const std::string& query) const {
        displayMatching(
            [&query](const Book& book) { return containsCaseInsensitive(book.getTitle(), query); },
            "--- Search Results (title) ---");
    }

    void Library::searchByAuthor(const std::string& query) const {
        displayMatching(
            [&query](const Book& book) { return containsCaseInsensitive(book.getAuthor(), query); },
            "--- Search Results (author) ---");
    }

    void Library::displayBooks() const {
        displayMatching([](const Book&) { return true; }, "--- All Books ---");
    }

    void Library::displayAvailable() const {
        displayMatching([](const Book& book) { return book.isAvailable(); },
                        "--- Available Books ---");
    }

    void Library::displayBorrowed() const {
        displayMatching([](const Book& book) { return !book.isAvailable(); },
                        "--- Borrowed Books ---");
    }

    bool Library::borrowBook(int id) {
        Book* book = findById(id);
        if (book == nullptr) {
            std::cerr << "No book found with ID " << id << ".\n";
            return false;
        }
        if (!book->isAvailable()) {
            std::cerr << "Book " << id << " is already borrowed.\n";
            return false;
        }

        book->borrow();
        saveBooks();
        std::cout << "Book " << id << " borrowed successfully.\n";
        return true;
    }

    bool Library::returnBook(int id) {
        Book* book = findById(id);
        if (book == nullptr) {
            std::cerr << "No book found with ID " << id << ".\n";
            return false;
        }
        if (book->isAvailable()) {
            std::cerr << "Book " << id << " is not currently borrowed.\n";
            return false;
        }

        book->returnBook();
        saveBooks();
        std::cout << "Book " << id << " returned successfully.\n";
        return true;
    }

    Book* Library::findById(int id) {
        for (auto& book : books_) {
            if (book->getId() == id) {
                return book.get();
            }
        }
        return nullptr;
    }

    const Book* Library::findById(int id) const {
        for (const auto& book : books_) {
            if (book->getId() == id) {
                return book.get();
            }
        }
        return nullptr;
    }
    // Finds the highest existing ID and generates the next available unique identifier.
    int Library::nextId() const {
        int maxId = 0;
        for (const auto& book : books_) {
            if (book->getId() > maxId) {
                maxId = book->getId();
            }
        }
        return maxId + 1;
    }
