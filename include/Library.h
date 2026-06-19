#pragma once

#include "Book.h"
#include "FileManager.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Manages the library's book collection and delegates persistence to FileManager.
class Library {
public:
    Library();

    void loadBooks();
    void saveBooks();

    bool addBook(const std::string& title, const std::string& author,
                 const std::string& type, const std::string& location);

    void searchByTitle(const std::string& query) const;
    void searchByAuthor(const std::string& query) const;
    void displayBooks() const;
    void displayAvailable() const;
    void displayBorrowed() const;

    bool borrowBook(int id);
    bool returnBook(int id);

private:
    std::vector<std::unique_ptr<Book>> books_;
    FileManager fileManager_;

    Book* findById(int id);
    const Book* findById(int id) const;
    int nextId() const;

    // Template helper: display books matching a predicate.
    template<typename Predicate>
    void displayMatching(Predicate pred, const std::string& header) const;
};

template<typename Predicate>
void Library::displayMatching(Predicate pred, const std::string& header) const {
    std::cout << '\n' << header << '\n';

    int found = 0;
    for (const auto& book : books_) {
        if (pred(*book)) {
            std::cout << *book << '\n';
            ++found;
        }
    }

    if (found == 0) {
        std::cout << "No books matched.\n";
    }
}
