#pragma once

#include "Book.h"

#include <memory>
#include <string>
#include <vector>

// Handles reading and writing book records to persistent storage.
class FileManager {
public:
    explicit FileManager(const std::string& path = "books.txt");

    std::vector<std::unique_ptr<Book>> readBooksFromFile();
    bool writeBooksToFile(const std::vector<std::unique_ptr<Book>>& books);
    bool updateBook(const Book& book);

private:
    std::string filepath_;
};
