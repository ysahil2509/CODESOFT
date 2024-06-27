#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

// Book class
class Book {
private:
    string title;
    string author;
    string isbn;
    bool available;

public:
    Book(const string& t, const string& a, const string& i)
        : title(t), author(a), isbn(i), available(true) {}

    // Getters
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    bool isAvailable() const { return available; }

    // Setters
    void setAvailable(bool avail) { available = avail; }
};

// Borrower class
class Borrower {
private:
    string name;
    string contact;

public:
    Borrower(const string& n, const string& c)
        : name(n), contact(c) {}

    // Getters
    string getName() const { return name; }
    string getContact() const { return contact; }
};

// Transaction class
class Transaction {
private:
    Borrower borrower;
    Book book;
    time_t checkoutDate;
    time_t returnDate;
    double fine;

public:
    Transaction(const Borrower& b, const Book& bk)
        : borrower(b), book(bk) {
        checkoutDate = time(nullptr); // Set current time as checkout date
        returnDate = 0;
        fine = 0.0;
    }

    const Book& getBook() const { return book; } // Getter for book

    // Calculate fine for overdue days
    void calculateFine() {
        const int secondsInDay = 86400; // 60 seconds * 60 minutes * 24 hours
        time_t now = time(nullptr);
        int daysOverdue = (now - returnDate) / secondsInDay;
        if (daysOverdue > 0) {
            fine = daysOverdue * 0.50; // Example: $0.50 per day
        }
    }

    // Setters
    void setReturnDate() {
        returnDate = time(nullptr); // Set current time as return date
    }

    // Getters
    double getFine() const { return fine; }
};

// Library class
class Library {
private:
    vector<Book> books;
    vector<Borrower> borrowers;
    vector<Transaction> transactions;

public:
    // Add book to library
    void addBook(const string& title, const string& author, const string& isbn) {
        books.push_back(Book(title, author, isbn));
    }

    // Search books by title, author, or ISBN
    void searchBooks(const string& keyword) {
        cout << "Search results:" << endl;
        for (const Book& book : books) {
            if (book.getTitle() == keyword || book.getAuthor() == keyword || book.getISBN() == keyword) {
                cout << "Title: " << book.getTitle() << ", Author: " << book.getAuthor() << ", ISBN: " << book.getISBN() << ", Available: " << (book.isAvailable() ? "Yes" : "No") << endl;
            }
        }
    }

    // Check out book to borrower
    void checkoutBook(int bookIndex, int borrowerIndex) {
        if (bookIndex >= 0 && bookIndex < books.size() && borrowerIndex >= 0 && borrowerIndex < borrowers.size()) {
            if (books[bookIndex].isAvailable()) {
                transactions.push_back(Transaction(borrowers[borrowerIndex], books[bookIndex]));
                books[bookIndex].setAvailable(false);
                cout << "Book checked out successfully." << endl;
            } else {
                cout << "Book is not available for checkout." << endl;
            }
        } else {
            cout << "Invalid book or borrower index." << endl;
        }
    }

    // Record book return
    void returnBook(int transactionIndex) {
        if (transactionIndex >= 0 && transactionIndex < transactions.size()) {
            transactions[transactionIndex].setReturnDate();
            transactions[transactionIndex].calculateFine();
            books.push_back(transactions[transactionIndex].getBook()); // Return book to library using getter
            transactions.erase(transactions.begin() + transactionIndex); // Remove transaction
            cout << "Book returned successfully." << endl;
        } else {
            cout << "Invalid transaction index." << endl;
        }
    }

    // Display all books and their availability
    void displayBooks() {
        cout << "Books in the library:" << endl;
        for (size_t i = 0; i < books.size(); ++i) {
            cout << i << ". Title: " << books[i].getTitle() << ", Author: " << books[i].getAuthor() << ", ISBN: " << books[i].getISBN() << ", Available: " << (books[i].isAvailable() ? "Yes" : "No") << endl;
        }
    }

    // Add borrower to library
    void addBorrower(const string& name, const string& contact) {
        borrowers.push_back(Borrower(name, contact));
    }

    // Display all borrowers
    void displayBorrowers() {
        cout << "Borrowers in the library:" << endl;
        for (size_t i = 0; i < borrowers.size(); ++i) {
            cout << i << ". Name: " << borrowers[i].getName() << ", Contact: " << borrowers[i].getContact() << endl;
        }
    }
};

// User Interface (console-based menu)
void displayMenu() {
    cout << "\n===== Library Management System =====" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Search Books" << endl;
    cout << "3. Checkout Book" << endl;
    cout << "4. Return Book" << endl;
    cout << "5. Display Books" << endl;
    cout << "6. Add Borrower" << endl;
    cout << "7. Display Borrowers" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Library library;
    int choice;
    string keyword;
    int bookIndex, borrowerIndex, transactionIndex;
    string title, author, isbn;
    string name, contact;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                library.addBook(title, author, isbn);
                break;
            case 2:
                cout << "Enter search keyword (title, author, or ISBN): ";
                cin >> keyword;
                library.searchBooks(keyword);
                break;
            case 3:
                library.displayBooks();
                cout << "Enter book index to checkout: ";
                cin >> bookIndex;
                library.displayBorrowers();
                cout << "Enter borrower index: ";
                cin >> borrowerIndex;
                library.checkoutBook(bookIndex, borrowerIndex);
                break;
            case 4:
                library.displayBooks();
                cout << "Enter transaction index to return: ";
                cin >> transactionIndex;
                library.returnBook(transactionIndex);
                break;
            case 5:
                library.displayBooks();
                break;
            case 6:
                cout << "Enter borrower name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter contact information: ";
                getline(cin, contact);
                library.addBorrower(name, contact);
                break;
            case 7:
                library.displayBorrowers();
                break;
            case 8:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 8." << endl;
        }

    } while (choice != 8);

    return 0;
}
