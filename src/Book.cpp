#include "Book.h"

#include <cctype>
#include <ctime>
#include <sstream>

namespace {
constexpr int BORROW_DAYS = 10;
constexpr long SECONDS_PER_DAY = 86400L;

const char* C_RESET   = "\033[0m";
const char* C_RED     = "\033[31m";
const char* C_GREEN   = "\033[32m";
const char* C_YELLOW  = "\033[33m";
const char* C_BLUE    = "\033[34m";
const char* C_MAGENTA = "\033[35m";
const char* C_CYAN    = "\033[36m";
const char* C_ORANGE  = "\033[38;5;208m";
const char* C_GRAY    = "\033[90m";

long daysRemaining(long borrowTime) {
    const long deadline = borrowTime + static_cast<long>(BORROW_DAYS) * SECONDS_PER_DAY;
    const long now = static_cast<long>(std::time(nullptr));
    return (deadline - now) / SECONDS_PER_DAY;
}
}  // namespace

Book::Book(int id, std::string title, std::string author,
           std::string type, std::string location,
           bool isBorrowed, long borrowTime)
    : id_(id),
      title_(std::move(title)),
      author_(std::move(author)),
      type_(std::move(type)),
      location_(std::move(location)),
      isBorrowed_(isBorrowed),
      borrowTime_(borrowTime) {}

void Book::borrow() {
    isBorrowed_ = true;
    borrowTime_ = static_cast<long>(std::time(nullptr));
}

void Book::returnBook() {
    isBorrowed_ = false;
    borrowTime_ = 0;
}

bool Book::isAvailable() const {
    return !isBorrowed_;
}

bool Book::isValidField(const std::string& value) {
    return !value.empty() && value.find('|') == std::string::npos;
}

std::string Book::getDetails() const {
    std::ostringstream oss;
    oss << "[ID: " << id_ << "] "
        << title_ << " by " << author_
        << " | " << type_ << " | " << location_ << " | ";

    if (isBorrowed_) {
        const long days = daysRemaining(borrowTime_);
        if (days < 0) {
            oss << "Borrowed (" << -days << " days overdue)";
        } else {
            oss << "Borrowed (" << days << " days left)";
        }
    } else {
        oss << "Available";
    }

    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "[ID: " << C_GRAY << book.id_ << C_RESET << "] "
       << C_CYAN << book.title_ << C_RESET << " by "
       << C_YELLOW << book.author_ << C_RESET << " | "
       << C_MAGENTA << book.type_ << C_RESET << " | "
       << C_BLUE << book.location_ << C_RESET << " | ";

    if (book.isBorrowed_) {
        const long days = daysRemaining(book.borrowTime_);
        if (days < 0) {
            os << C_RED << "Borrowed" << C_RESET << " ("
               << C_RED << -days << " days overdue" << C_RESET << ")";
        } else {
            const char* daysColor = (days <= 2) ? C_ORANGE : C_YELLOW;
            os << C_RED << "Borrowed" << C_RESET << " ("
               << daysColor << days << " days left" << C_RESET << ")";
        }
    } else {
        os << C_GREEN << "Available" << C_RESET;
    }

    return os;
}
