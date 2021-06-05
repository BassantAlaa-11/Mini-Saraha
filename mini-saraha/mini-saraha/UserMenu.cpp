#include "UserMenu.h"
#include <iostream>
#include "Server.h"
#include "WelcomeMenu.h"
using namespace std;

User* userP;
Server* serverP;

UserMenu::UserMenu(Server &server) {
    serverP = &server;
    userP = server.get_Current_Logged_User();
}

void addToFavorite(int index)
{
    //since first message displayed is last message in array
    // and assuming user input is one based
    int size = userP->getRecievedMessages().size();
    int target = size - index; 
    
    serverP->addFavoriteMessage(userP->getReceivedMessage(target));
}

void viewRecivedMessages( ) {
    if (userP->foundMessages())
    {
        serverP->viewMessages(userP->getID());
        cout << "----------------------" << endl;
        cout << "[1] Add Message To Favourite " << endl;
        cout << "[2] Back To Main Menu " << endl;
        cout << "----------------------" << endl;
        cout << "Your Choice : ";
        int userChoice;    cin >> userChoice;


        switch (userChoice) {
        case 1:
            cout << "Choose Message :"; cin >> userChoice;
            addToFavorite(userChoice);
            break;
        case 2:
            return;

        default:
            break;
        }
    }
    else
    {
        cout << "You don't have any messages" << endl; 
    }
}


void viewSentMessages(tgui::ListBox::Ptr messageList) {

    if (userP->foundSentMessages())
    {
        int y = userP->getSentMessages().size();
        for (int i = 0; i < userP->getSentMessages().size(); i++) {
            messageList->addItem(tgui::String(userP->getSentMessages()[i].getMessageBody()));
        }
    }
    else {
        messageList->addItem(tgui::String("You don't have any messages"));
    }
}

void sentMessagesWidgets(tgui::GuiBase& gui) {
    gui.removeAllWidgets();
    gui.loadWidgetsFromFile("sent.txt");

    auto messageList = gui.get<tgui::ListBox>("messageList");
    viewSentMessages(messageList);

    auto undo = gui.get<tgui::Button>("Button1");
    undo->onPress([&] {serverP->deleteLastMessage(); });

    auto back = gui.get<tgui::Button>("back");
    back->onPress([&] {UserMenu::backi(gui); });
}


void viewFavouriteMessages() {

    
    if (userP->foundFavouriteMessages())
    {

        userP->showfavoriteMassages();
        cout << "----------------------" << endl;
        cout << "[1] Delete Oldest Favourite Message " << endl;
        cout << "[2] Back To Main Menu " << endl;
        cout << "------------------------------- " << endl;
        cout << "Your Choice : ";
        int userChoice;    cin >> userChoice;
        switch (userChoice) {
        case 1:
            serverP->delete_Last_Favorite_Message();
            break;
        case 2:
            return;

        default:
            break;


        }
    }
    else {
        cout << "You don't have any messages" << endl;
    }
}


void displayUserData(User  user) {

    cout << "------------------------" << endl;
    cout << "Username : " << user.getUsername()<< endl; 
    cout << "id : " << user.getID() << endl;
    cout << "------------------------" << endl;
}

void usersSearch(tgui::EditBox::Ptr id, tgui::Label::Ptr result, tgui::Button::Ptr add) {
    
    int user_id = id->getText().toInt();
    /*cout << "------SEARCH-------" << endl;
    cout << "Enter user id : ";   cin >> user_id;*/ 

    User *userS = serverP->findUser(user_id);
    if (userS != nullptr) {
        add->setVisible(true);
        result->setText(userS->getUsername()+ "#" + to_string(userS->getID()));
        add->onPress([&] { serverP->addContact(userS->getID()); });
    }
    else {
        result->setText("No user with this id");
        add->setVisible(false);
    }
 
}

void searchWidgets(tgui::GuiBase& gui) {
    gui.removeAllWidgets();
    gui.loadWidgetsFromFile("search.txt");

    auto searchBox = gui.get<tgui::EditBox>("search");
    auto result = gui.get<tgui::Label>("screen");
    auto addContact = gui.get<tgui::Button>("Button1");
    addContact->setVisible(false);
    searchBox->onReturnKeyPress(&usersSearch, searchBox, result, addContact);
    auto back = gui.get<tgui::Button>("back");
    back->onPress([&] {UserMenu::backi(gui); });
}

void sendMessage(User user)
{
    cout << "Tell "<< user.getUsername() << " anything!" << endl;
    string message; 
    cin >> message; 
    Message messageS(userP->getID(), user.getID(), message);
    serverP->sendMessage(messageS);

}



void selectContact()
{
    cout << "Enter User id : ";
    int user_id;    cin >> user_id;
    User selected_contact = *serverP->findUser(user_id);
        cout << "[1] Send Message:" << endl;
        cout << "[2] View Contacts :" << endl;
        cout << "------------------------" << endl;
        cout << "Your Choice : ";

        int userChoice;    cin >> userChoice;

        switch (userChoice)
        {
        case 1:
            sendMessage(selected_contact);
            break;
        case 2:
            selected_contact.showContacts();
            cout << "[1]To main menu: ";
            cin >> userChoice; 
            return;

        default:
            break;
        }
    }
   



void viewContacts() {
     
    if (userP->foundContacts())
    {

        cout << "------------------------" << endl;
        cout << "     Contacts  " << endl;
        cout << "------------------------" << endl;
        userP->showContacts();
        cout << "------------------------" << endl;
        cout << "[1] Select Contact  " << endl;
        cout << "[2] Back To Main Menu " << endl;
        cout << "------------------------" << endl;
        cout << "Your Choice : ";


        int userChoice;    cin >> userChoice;
        switch (userChoice)
        {
        case 1:
            selectContact();
            break;
        case 2:
            return;

        default:
            break;
        }
    }
}


void UserMenu::initial(tgui::GuiBase& gui)
{
    gui.removeAllWidgets();
    gui.loadWidgetsFromFile("user.txt");
    auto widgets = gui.getWidgets();
    auto label = gui.get<tgui::Label>("label");
    label->setText("WELCOME TO YOUR ACCOUNT " + to_string(userP->getID()));

    auto myMessages = gui.get<tgui::Button>("my_messages");
    myMessages->onPress(&viewRecivedMessages);

    auto sent = gui.get<tgui::Button>("sent");
    sent->onPress([&] {sentMessagesWidgets(gui); });

    auto fav = gui.get<tgui::Button>("fav");
    fav->onPress(&viewFavouriteMessages);

    auto search = gui.get<tgui::Button>("search");
    search->onPress([&] {searchWidgets(gui); });

    auto contacts = gui.get<tgui::Button>("contacts");
    contacts->onPress(&viewContacts);

    auto logout = gui.get<tgui::Button>("logout");
    logout->onPress([&] {serverP->saveSession(); WelcomeMenu::backi(gui); });
}

void UserMenu::backi(tgui::GuiBase& gui)
{
    initial(gui);
}
