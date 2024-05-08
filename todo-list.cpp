#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include "json.hpp"
using json = nlohmann::json;

std::string file_name = "todo.json";
std::ifstream file;
json data;

class TodoApp {
public:
    
    
    void initialize() {
        if (!file.is_open()) {
            std::cout << "No previous file found: Creating a new todo file..." << std::endl;
            std::ofstream newFile(file_name);
            json defaultContent = {
                    {"status", "active"},
                    {"todos", json::array({
                        {
                            {"name", "Your first todo"},
                            {"status", "In-progress"},
                            {"ID", 1}
                        }
                    })}};
            newFile << defaultContent.dump(4);

        } else {
            show_todo();
        }
    }
    void reload(){
        file.close();
        file.open(file_name);
            if (!file) {
                std::cerr << "Error opening file: " << file_name << std::endl;
                TodoApp:: exit();
            }
    }

    void exit(){
        file.close();
    }

    void add_todo(){
        json newTodo;
        std::string name, status;
        int id;
        std::cout << "Please enter the name of the todo: ";
        std::getline(std::cin >> std::ws, name);

        std::cout << "Please enter the status of the todo: ";
        std::getline(std::cin >> std::ws, status);

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(1, 10000);
        id = distribution(generator);
        
        newTodo = {
            {"ID", id},
            {"name", name},
            {"status", status},
            
        };
        data["todos"].push_back(newTodo);
        std::ofstream output(file_name);
        output << data.dump(4);
        output.close();
        
    }
    void update_todo(json& data){
        int option;
        int id;
        std::cout << "Please give me todo ID : ";
        std::cin >> id;
        std::cout << "\nPlease select from the following update options:\n";
        std::cout << "\n1: Rename";
        std::cout << "\n2: Change Status\n";
        std::cin >> option;
        if (option == 1){
            std::string name;
            std::cout << "\nPlease give me the new name : ";
            std::cin >> name;
            update_name(data, id, name);
        }
        else if(option == 2){
            std::string status;
            std::cout << "\n Please give me the new status : ";
            std::cin >> status;
            update_name(data, id, status);
        }
    }
    void delete_todo(){
        
    }
    void extract_todo(json& data){
        json todos = data["todos"];
        std::string markdown = ("# Todo-List \n");
        std::ofstream md("markdown_todos.md");
        markdown += "\n\n| :rotating_light: ID | :office: Name | :clipboard: Status |\n| :---------------: | :-----------: | :-----------------: |\n";

        for (json t : todos) {
            std::string id = std::to_string(static_cast<int>(t["ID"]));
            std::string name = t["name"];
            std::string status = t["status"];
            markdown += "| " + id + " | " + name + " | " + status + " |\n";
        }
        md << markdown;
        md.close();
    }
    void update_name(json& data,int id, std:: string name){
        json todos = data["todos"];
        int match = 0;
        for (json& todo : todos) {
            if (todo["ID"] == id) {
                std::cout << "Found todo with name: " << todo["name"] << std::endl;
                todo["name"] = name;
                std::ofstream output(file_name, std::ios::out | std::ios::trunc);
                output << data.dump(4);
                output.close();
                match++;
                break; 
            }
        }
        if (match == 0) {
            std::cout << "\nNo value found for that ID" << std::endl;
        } else {
            reload();
        }
    }
    void update_status(json& data, int id, std:: string status){
        json todos = data["todos"];
        std::cout << todos;

        for (json& todo : todos){
            if (todo["ID"] == id){
                todo["status"] = status;
                std::ofstream output(file_name);
                output << data.dump(4);
            }
        }  
    }

    void show_todo() {
        data = json::parse(file);
        int action=1;
        std::string input;
        bool run = true;
        std::cout << run << " " << action << std::endl;
        while (run){
            std::cout << "\n\nYour current Todos are listed below:\n\n " << std::endl;
            if (!data.empty()) {
                for (json& todo : data["todos"]){
                    std:: cout << "ID: " << todo["ID"] << " Name: " << todo["name"].get<std::string>() << " Status: " << todo["status"].get<std::string>() << std::endl;

                }
            }
            std::cout << "\n\nPlease select an action to continue." << std::endl; 
            std::cout << "1: Add a new todo\n" << "2: Update a todo\n" << "3: Delete a todo\n" << "4: Extract a pdf\n" << "0: Exit" << std::endl;
            std::getline(std::cin, input);
            std::stringstream(input) >> action;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << action << std::endl;
            if (std::stringstream(input) >> action) {
                switch (action) {
                    case 1:
                        add_todo();
                        break;
                    case 2:
                        update_todo(data);
                        break;
                    case 3:
                        delete_todo();
                        break;
                    case 4:
                        extract_todo(data);
                        break;
                    case 0:
                        run = false;
                        TodoApp::exit();
                        break;
                    default:
                        std::cout << "\nERROR: Unknown action";
                        break;
                }
            } else {
                std::cout << "Invalid input. Please enter a number." << std::endl;
            }
        }
    }
};


int main() {
    TodoApp app;
    file.open(file_name);
    app.initialize();
    return 0;
}
