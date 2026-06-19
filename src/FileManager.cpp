#include "FileManager.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace {
// Splits a pipe-delimited book record into its seven fields.
bool parseBookLine(const std::string& line, int& id, std::string& title,
                   std::string& author, std::string& type, std::string& location,
                   bool& isBorrowed, long& borrowTime) {
    std::istringstream iss(line);
    std::string token;
    std::string tokens[7];
    int count = 0;

    while (std::getline(iss, token, '|') && count < 7) {
        tokens[count++] = token;
    }

    if (count != 7) {
        return false;
    }

    try {
        id = std::stoi(tokens[0]);
        title = tokens[1];
        author = tokens[2];
        type = tokens[3];
        location = tokens[4];
        isBorrowed = (std::stoi(tokens[5]) != 0);
        borrowTime = std::stol(tokens[6]);
    } catch (const std::exception&) {
        return false;
    }

    return true;
}

std::string formatBookLine(const Book& book) {
    std::ostringstream oss;
    oss << book.getId() << '|'
        << book.getTitle() << '|'
        << book.getAuthor() << '|'
        << book.getType() << '|'
        << book.getLocation() << '|'
        << (book.getIsBorrowed() ? 1 : 0) << '|'
        << std::setfill('0') << std::setw(10) << book.getBorrowTime();
    return oss.str();
}
}  // namespace

FileManager::FileManager(const std::string& path) : filepath_(path) {}

std::vector<std::unique_ptr<Book>> FileManager::readBooksFromFile() {
    std::vector<std::unique_ptr<Book>> books;
    std::ifstream file(filepath_);

    if (!file.is_open()) {
        return books;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        int id = 0;
        std::string title, author, type, location;
        bool isBorrowed = false;
        long borrowTime = 0;

        if (parseBookLine(line, id, title, author, type, location, isBorrowed, borrowTime)) {
            books.push_back(std::make_unique<Book>(
                id, title, author, type, location, isBorrowed, borrowTime));
        }
    }

    return books;
}

bool FileManager::writeBooksToFile(const std::vector<std::unique_ptr<Book>>& books) {
    std::ofstream file(filepath_);
    if (!file.is_open()) {
        return false;
    }

    file << "# id|title|author|type|location|isBorrowed|borrowTime\n";
    for (const auto& book : books) {
        file << formatBookLine(*book) << '\n';
    }

    return file.good();
}

bool FileManager::updateBook(const Book& book) {
    std::fstream file(filepath_, std::ios::in | std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::streampos lineStart = file.tellg();

    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            int id = 0;
            if (line.find('|') != std::string::npos) {
                try {
                    id = std::stoi(line.substr(0, line.find('|')));
                } catch (const std::exception&) {
                    lineStart = file.tellg();
                    continue;
                }
            }

            if (id == book.getId()) {
                const std::string updated = formatBookLine(book);
                file.seekp(lineStart);
                file << updated;
                if (static_cast<std::streamsize>(updated.size()) < static_cast<std::streamsize>(line.size())) {
                    file << std::string(line.size() - updated.size(), ' ');
                }
                file << '\n';
                return file.good();
            }
        }
        lineStart = file.tellg();
    }

    return false;
}
