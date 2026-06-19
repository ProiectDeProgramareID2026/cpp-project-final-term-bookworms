#pragma once

#include <iostream>
#include <string>

// Represents a single book in the library collection.
class Book {
public:
    Book(int id, std::string title, std::string author,
         std::string type, std::string location,
         bool isBorrowed = false, long borrowTime = 0);

    void borrow();
    void returnBook();
    bool isAvailable() const;
    std::string getDetails() const;

    // Validates book text fields (non-empty, no pipe delimiter).
    static bool isValidField(const std::string& value);

    int getId() const { return id_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getAuthor() const { return author_; }
    const std::string& getType() const { return type_; }
    const std::string& getLocation() const { return location_; }
    bool getIsBorrowed() const { return isBorrowed_; }
    long getBorrowTime() const { return borrowTime_; }

    friend std::ostream& operator<<(std::ostream& os, const Book& book);

private:
    int id_;
    std::string title_;
    std::string author_;
    std::string type_;
    std::string location_;
    bool isBorrowed_;
    long borrowTime_;
};
