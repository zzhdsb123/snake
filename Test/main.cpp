//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
using namespace std;

class Snake{
private:
    void move_right(){
    position.erase(position.begin());
    vector<int> temp = position[position.size()-1];
    temp[0] += 1;
    position.push_back(temp);
}
    
    void move_left(){
        position.erase(position.begin());
        vector<int> temp = position[position.size()-1];
        temp[0] -= 1;
        position.push_back(temp);
    }
    
    void move_up(){
        position.erase(position.begin());
        vector<int> temp = position[position.size()-1];
        temp[1] -= 1;
        position.push_back(temp);
    }
    
    void move_down(){
        position.erase(position.begin());
        vector<int> temp = position[position.size()-1];
        temp[1] += 1;
        position.push_back(temp);
    }
    
public:
    int run = 1;
    
    int death = 0;
    
    int score = 0;
    
    vector<vector<int>> position = {{10,11},{11,11},{12,11},{13,11}};
    
    vector<vector<int>> pre;
    
    int direction = 6;
    
    set<vector<int>> apple;
    
    Snake(){
        for (int i=0; i<40; ++i){
            for (int j=0; j<40; ++j){
                vector<int> temp = {i,j};
                apple.insert(temp);
            }
        }
    }
    
    vector<int> generate_apple(){
        vector<vector<int>> :: iterator it;
        for (it = position.begin(); it != position.end(); ++it){
            apple.erase(*it);
        }
        int mini = 0;
        int maxi = apple.size();
        int index = rand()%(maxi - mini + 1) + mini;
        vector<vector<int>> candidate(apple.begin(),apple.end());
        return candidate[index];
    }
    
    vector<int> get_head(){
        return position[position.size()-1];
    }
    
    void move(){
        switch (direction) {
            case 8:
                move_up();
                break;
            
            case 2:
                move_down();
                break;
                
            case 4:
                move_left();
                break;
                
            case 6:
                move_right();
                break;
                
            default:
                break;
        }
    }
    
    void grow(){
        score += 1;
        vector<int> temp = position[0];
        switch (direction) {
            case 8:
                temp[1] += 1;
                position.insert(position.begin(), temp);
                break;

            case 2:
                temp[1] -= 1;
                position.insert(position.begin(), temp);
                break;
                
            case 4:
                temp[0] += 1;
                position.insert(position.begin(), temp);
                break;
                
            case 6:
                temp[0] -= 1;
                position.insert(position.begin(), temp);
                break;
                
            default:
                break;
        }
    }
    
    void stop(){
        vector<int> snake_head = get_head();
        if (snake_head[0] < 0 or snake_head[0] >= 40 or snake_head[1] < 0 or snake_head[1] >= 40){
            run = 0;
            position = pre;
        }
        vector<vector<int>>::iterator it = find(position.begin(), position.end(), snake_head);
        if (distance(position.begin(), it) != position.size()-1){
            run = 0;
            position = pre;
            // cout << "shit" << endl;
        }
    }
    
    int find_direction(){
        vector<int> first = position[position.size()-1];
        vector<int> second = position[position.size()-2];
        if (first[0]-second[0] == 1){
            return 6;
        }
        if (first[0]-second[0] == -1){
            return 4;
        }
        if (first[1]-second[1] == 1){
            return 2;
        }
        if (first[1]-second[1] == -1){
            return 8;
        }
    }
};

int main(int, char const**)
{
    string over;
    
    sf::RenderWindow window(sf::VideoMode(1200, 1200), "Snake");

    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "ppz.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Texture snake_part;
    if (!snake_part.loadFromFile(resourcePath() + "snake_part.png")) {
        return EXIT_FAILURE;
    }

    
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Music music;
    if (!music.openFromFile(resourcePath() + "bgm.ogg")) {
        return EXIT_FAILURE;
    }
    
    sf::Music death;
    if (!death.openFromFile(resourcePath() + "death.ogg")){
        return -1;
    }
    
    Snake snake;
    sf::Clock clock;
    music.play();
    music.setLoop(true);
    sf::Sprite fuck(snake_part);
    vector<int> apple = snake.generate_apple();
    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed){
                switch (event.key.code) {
                    case sf::Keyboard::Right:{
                        if (snake.find_direction() != 4){
                                snake.direction = 6;
                            }
                            break;
                    }
                        
                    case sf::Keyboard::Left:{
                        if (snake.find_direction() != 6){
                            snake.direction = 4;
                        }
                        break;
                    }
                        
                    case sf::Keyboard::Up:{
                        if (snake.find_direction() != 2){
                            snake.direction = 8;
                        }
                        break;
                    }
                        
                    case sf::Keyboard::Down:{
                        if (snake.find_direction() != 8){
                            snake.direction = 2;
                        }
                        break;
                    }
                        
                    case sf::Keyboard::Enter:{
                        music.play();
                        if (not snake.run){
                            Snake new_snake;
                            snake = new_snake;
                            death.stop();
                        }
                        break;
                    }
                        
                    case sf::Keyboard::Escape:{
                        window.close();
                        break;
                    }
                        
                    default:
                        break;
                }
            }
        }

        if (not snake.run and not snake.death){
            death.play();
            snake.death = 1;
        }
        
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed.asSeconds() > 0.05){
            if (snake.run){
                snake.pre = snake.position;
                snake.move();
                clock.restart();
            }
            snake.stop();
        }
        
        vector<int> head = snake.get_head();
        if (head == apple){
            apple = snake.generate_apple();
            snake.grow();
        }
        
        window.clear();
        
        for (int i=0; i<snake.position.size(); ++i){
            sf::Sprite shit(snake_part);
            shit.setPosition(30.0f * snake.position[i][0], 30.0f * snake.position[i][1]);
            window.draw(shit);
        }
        
        if (snake.run == 0){
            over = " Press Enter to Restart!";
            music.stop();
        }
        else{
            over = "";
        }
        
        sf::Text text("YOUR SCORE: " + to_string(snake.score) + over, font, 50);
        text.setFillColor(sf::Color::White);
        window.draw(text);


        fuck.setPosition(30.0f * apple[0], 30.0f * apple[1]);
        window.draw(fuck);

        
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
