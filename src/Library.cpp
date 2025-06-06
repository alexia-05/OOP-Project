#include "Library.h"
#include <iostream>
#include <limits>
#include <memory>
#include "Student.h"
#include "AveragePerson.h"
#include "Poetry.h"
#include "Novel.h"
#include "Reader.h"
#include "FilterBooks.h"
#include "PersonBuilder.h"
#include "StudentBuilder.h"
#include "Admin.h"
#include "Template.h"
#include "Book.h"
#include "InvalidNumber.h"
#include "SearchStrategy.h"
#include "BookFactory.h"
#include <fstream>
#include <sstream>

Library* Library::instance=nullptr;
Library& Library::getInstance()
{
    if (instance==nullptr)
        instance=new Library();
    return *instance;
}
int Library::validateChoice(const int min, const int max)
{  const int maxTries=3;// userul poate pune gresit de max 3 ori
    int tries=0;
    int choice;
    while (tries<maxTries)
    {
        std::cin>>choice;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"Invalid choice! Please enter a number between "<<min<<" and "<<max<<" ( or press '0' to exit)\n";
            tries++;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice==0)
            return -1;
        if (choice<min || choice>max)
        {
            std::cout<<"Invalid choice! Please enter a number between "<<min<<" and "<<max<<" ( or press '0' to exit)\n";
            tries++;
            continue;
        }
        return choice;

    }
    std::cout<<"Too many tries\n";
    return -1;
}


void Library::addData()
{
    StudentBuilder builderS;
    PersonBuilder builderP;
    std::fstream fin;
    fin.open("../txt_files/Readers.csv");

    std::string line;
    std::getline(fin, line); // prima linie

    while (std::getline(fin, line))
    {
        std::string type, firstName, lastName, age, grade, occupation;
        std::stringstream s(line);
        std::getline(s, type, ',');
        std::getline(s, firstName, ',');
        std::getline(s, lastName, ',');
        std::getline(s, age, ',');
        std::getline(s, grade, ',');
        std::getline(s, occupation, ',');

        int age_=0;
        int grade_ = 0;

        if (age.empty()==false)
            age_=std::stoi(age);
        if (grade.empty()==false)
            grade_=std::stoi(grade); // din string in int

        if (type == "student")
        {
            Student stud = builderS.addFirstName(firstName).addLastName(lastName).addAge(age_).addGrade(grade_).build();
            readerDb.addItem(std::make_shared<Student>(stud));
        }

        else if (type == "person")
        {
            AveragePerson pers = builderP.addFirstName(firstName).addLastName(lastName).addAge(age_).addOccupation(occupation).build();
            readerDb.addItem(std::make_shared<AveragePerson>(pers));
        }

    }
    fin.close();

    fin.open("../txt_files/Books.csv");
    std::getline(fin, line); // prima linie
    while (std::getline(fin, line))
    {
        std::string type, name, author, genre, year, copies, poems, pages, chapters;
        std::stringstream s(line);
        std::getline(s, type, ',');
        std::getline(s, name, ',');
        std::getline(s, author, ',');
        std::getline(s, genre, ',');
        std::getline(s, year, ',');
        std::getline(s, copies, ',');
        std::getline(s, poems, ',');
        std::getline(s, pages, ',');
        std::getline(s, chapters, ',');

        int year_,copies_, poems_, pages_, chapters_ = 0;

        if (year.empty()==false)
            year_=std::stoi(year);
        if (copies.empty()==false)
            copies_=std::stoi(copies);
        if (poems.empty()==false)
            poems_=std::stoi(poems);
        if (pages.empty()==false)
            pages_=std::stoi(pages);
        if (chapters.empty()==false)
            chapters_=std::stoi(chapters);

        if (type=="poetry")
            bookDb.addItem(std::make_shared<Poetry>(name,author, genre, year_, copies_, poems_));
        else if (type=="novel")
            bookDb.addItem(std::make_shared<Novel>(name,author, genre, year_, copies_, pages_, chapters_));
    }
    fin.close();

    // for (const auto& reader : readerDb)
    //     reader->display(std::cout);
    //
    // for (const auto& book : bookDb)
    //
    //     book->display(std::cout);


}
// { Varianta inainte sa mi iasa cu fisiere
//     StudentBuilder builderS;
//     PersonBuilder builderP;
//
//     Student s1 = builderS.addFirstName("Alex").addLastName("Popescu").addAge(19).addGrade(11).build();
//     Student s2 = builderS.addFirstName("Maria").addLastName("Ionescu").addAge(20).addGrade(12).build();
//
//     AveragePerson p1 = builderP.addFirstName("Ana").addLastName("Marinescu").addAge(45).addOccupation("Teacher").build();
//     AveragePerson p2 = builderP.addFirstName("Mihai").addLastName("Popa").addAge(70).addOccupation("Driver").build();
//
//     readerDb.addItem(std::make_shared<Student>(s1));
//     readerDb.addItem(std::make_shared<Student>(s2));
//     readerDb.addItem(std::make_shared<AveragePerson>(p1));
//     readerDb.addItem(std::make_shared<AveragePerson>(p2));
//
//     for (const auto& reader : readerDb )
//         reader->display(std::cout);
//
//     bookDb.addItem(std::make_shared<Poetry>("Luceafarul", "Mihai Eminescu", "Poezie romantica", 1883, 1, 98));
//     bookDb.addItem(std::make_shared<Novel>("Harry Potter and the Philosopher's Stone ", "J.K Rowling", "Fantasy", 1997, 1, 320, 17));
//
// }
void Library::removeBook()
{   std::cout<<"Enter the ID of the book you want to delete: \n";
    const int id=validateChoice( 1, bookDb.size());
    if (id==-1)
        return ;
    if (!bookDb.removeItem(id))
        std::cout<<"No book found with ID: "<<id<<std::endl;
    else
        std::cout<<"Book deleted! \n";
    // for (const auto& i : bookDb )
    //     i->display(std::cout);
}

void Library::addReader(std::istream& is)
{
    int choice;
    std::shared_ptr<Reader> reader;
    while (true)
    {
        std::cout<<"Pick your membership:\n"
                   "1. Student \n"
                   "2. Normal Membership \n"
                   "3. Cancel\n";
        std::cin>>choice;
    //chestie cautata pe google (Geeks for geeks)
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"Invalid choice! Please choose 1, 2 or 3.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            reader = std::make_shared<Student>(" ", " ", 0, 0);
            reader->addReader(is);
            break;
        case 2:
            reader = std::make_shared<AveragePerson>(" ", " ", 0, " ");
            reader->addReader(is);
            break;
        case 3:
            return;
        default:
            std::cout<<"Invalid choice! Please choose 1 or 2\n";
            continue;
        }
        break;
    }
    readerDb.addItem(reader);
    // for (const auto& i : readerDb )
    //     i->display(std::cout);

}
void Library::searchBook() const
{
    std::shared_ptr<SearchStrategy> search;
       try
       {
           int choice;
           std::cout<<"Search by:\n 1. Book name \n 2. Author\n 3.Genre \n";
           std::cin>>choice;
           if (std::cin.fail())
           {
               std::cin.clear();
               std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
               throw InvalidChoice();
           }
           if (choice==1)
               search=std::make_shared<SearchByName>();
           else if (choice==2)
               search=std::make_shared<SearchByAuthor>();
           else if (choice==3)
               search=std::make_shared<SearchByGenre>();
           else
               throw InvalidChoice();
       }
        catch (InvalidChoice& e)
       {
           std::cout<<e.what()<<"\n";
           return;
       }
        std::string searchTerm;
        std::cout<<"Enter search term: \n";
        std::cin.ignore();
        std::getline(std::cin, searchTerm);
        std::vector<std::shared_ptr<Book>> books=search->search(bookDb.getItems(),searchTerm);
        if (books.empty())
            std::cout<<"No books found!\n";
        else
            for (const auto& book : books)
                book->display(std::cout);
}

bool Library::adminMenu()
{
    if (admin.login())
    {
        while (true)
        {   std::cout<<"Hi admin! What do you want to do?\n";
            std::cout<<"1. Add book\n";
            std::cout<<"2. Remove book\n";
            std::cout<<"3. Display books\n";
            std::cout<<"4. Display readers\n";
            std::cout<<"5. Search a book\n";
            std::cout<<"6. Exit\n";
            const int choice = validateChoice(1, 6);
            if (choice==-1)
            {
                std::cout<<"am iesit\n";
                return false;
            }
            switch (choice)
            {
                case 1:
                    addBook(std::cin);
                    break;
                case 2:
                    removeBook();
                    break;
                case 3:
                    bookDb.displayItems();
                    break;
                case 4:
                    readerDb.displayItems();
                    break;
                case 5:
                    searchBook();
                    break;
                case 6:
                    return true;
                default:
                    std::cout<<"Invalid input! Please choose 1, 2, 3, 4 or 5\n";
                    break;

            }
        }
    }
        return false;
}
void Library::addBook(std::istream& is)
{
    std::cout<<"What do you want to add? (1 for Poetry and 2 for Novel)\n";
    const int type = validateChoice( 1, 2);
    if (type==-1)
        return;
    auto book=BookFactory::createBook(is, type);
    if (book!=nullptr)
    {
        bookDb.addItem(book);
        std::cout<<"Book added! \n";
    }
    else
        std::cout<<"Book adding failed.\n";

    // for (const auto& i : bookDb )
    //     i->display(std::cout);

}
std::shared_ptr<Reader> Library::loginUser() const
{
    std::cout<<"Enter your ID: ";
    int id;
    std::cin>>id;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Invalid ID! \n";
        return nullptr;
    }
    if (auto user=readerDb.findId(id))
    {
        std::cout<<"Welcome "<<user->getFirstName()<<" "<<user->getLastName()<<"!\n";
        userMenu2(user);
        return user;
    }
    std::cout<<"No user found with ID: "<<id<<"\n";
    return nullptr;
}
void Library::userMenu()
{
    std::cout<<"=========================\n";
    std::cout<<"         USER MENU       \n";
    std::cout<<"=========================\n";
    std::cout<<"Do you already have a membership? (y/n)\n";
    std::string input;
    while (true)
    {
        std::cin>>input;
        if (input=="y" || input=="Y")
        {
            if (const auto user=loginUser())
                userMenu2(user);
            else
                std::cout<<"Login failed. Please try again or create a membership.\n";
            break;
        }
        if (input=="n" || input=="N")
         {
             addReader(std::cin);
             break;
         }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Invalid input! Please enter 'y' or 'n'. \n";
    }
}
// bool Library:: stringInput(std::istream& is, std::string& input)
// {
//     while (true)
//     {
//         std::getline(is, input);
//         bool stringValidate = std::find_if(input.begin(), input.end(),
//                        [](char c) { return !std::isalpha(c); }) != input.end();
//         if (stringValidate==false)
//             return true;
//         std::cout<<"Invalid input! Please enter a valid name (letters only).\n";
//
//
//     }
// }

void Library::userMenu2(const std::shared_ptr<Reader>& user) const
{
    while (true)
    {
        std::cout<<"=========================\n";
        std::cout<<"         USER MENU       \n";
        std::cout<<"=========================\n";
        std::cout<<"1. Display books\n";
        std::cout<<"2. Display borrowed books\n";
        std::cout<<"3. Borrow a book \n";
        std::cout<<"4. Return a book\n";
        std::cout<<"5. Search a book\n";
        std::cout<<"6. Exit\n";
        std::cout<<"=========================\n";
        std::cout<<"What do you want to do?\n";
        const int choice=validateChoice( 1, 6);
        if (choice==-1)
            return;
        switch (choice)
        {
        case 1:
            bookDb.displayItems();
            break;
        case 2:
            user->displayBorrowedBooks();
            break;
        case 3:
            while (true)
            {
                std::cout<<"Sort by:\n 1. None \n 2.Name\n 3.Release Year\n";
                const int sortChoice=validateChoice(1, 3);
                if (sortChoice==-1)
                    return;
                if (sortChoice==1)
                    bookDb.displayItems();
                else
                {
                    const SortBy by=(sortChoice==2)? SortBy::Name : SortBy::Year;
                    while (true)
                    {
                        std::cout<<"Press 'a' (for ascending), 'd' (for descending) or 'e' to exit.\n";
                        std::string input;
                        std::cin>>input;
                        if (input=="a" || input=="A")
                        {
                            std::vector<std::shared_ptr<Book>> sortedBooks=FilterBooks::filter(bookDb.getItems(), by, SortOrder::Ascending);
                            for (const auto& book : sortedBooks)
                                book->display(std::cout);
                            break;
                        }
                        if (input=="d" || input=="D")
                        {
                            std::vector<std::shared_ptr<Book>> sortedBooks=FilterBooks::filter(bookDb.getItems(), by, SortOrder::Descending);
                            for (const auto& book : sortedBooks)
                                book->display(std::cout);
                            break;
                        }
                        if (input=="e" || input=="E")
                                return;

                        std::cout<<"Invalid input! Please enter 'a', 'd' or 'e'. \n";
                    }
                }
                std::cout<<"Enter the ID of the book you want to borrow: \n";
                const int bookId=validateChoice( 1, bookDb.size());
                if (bookId==-1)
                    return;
                auto book=bookDb.findId(bookId);
               if (book!=nullptr)
                   user->borrowBook(book);
               else
                    std::cout<<"No book found with ID: "<<bookId<<"\n";
                break;
            }
            break;
        case 4:
            {
                user->displayBorrowedBooks();
                std::cout<<"Enter ID of book to return: ";
                const int bookId=validateChoice(1, bookDb.size());
                if (bookId==-1)
                    return;
                auto book=bookDb.findId(bookId);
                user->returnBook(*book);
                break;
            }
        case 5:
            searchBook();
            break;
        case 6:
            return;
        default:
            std::cout<<"Invalid input. Please choose a number between 1 and 6.\n";
            break;

        }
    }
}
void Library::start()
{   do {
        std::cout << "=========================\n";
        std::cout << "Welcome to the library!\n";
        std::cout << "=========================\n";
        std::cout << "1. Login user\n";
        std::cout << "2. Login admin\n";
        std::cout << "3. Exit\n";
        int choice;
        choice = validateChoice(1, 3);
        switch (choice) {
            case 1:
                userMenu();
                break;
            case 2:
                if(!adminMenu())
                    break;
                break;
            case 3:
                std::cout << "Have a nice day!!\n";
                return;
            default:
                break;
        }
    }while(true);
}
