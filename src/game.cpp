#include "game.hpp"
#include <box2d/box2d.h>


Game::Game() 
    : window_(sf::VideoMode(1366, 768), "Angry Birds game", sf::Style::Titlebar | sf::Style::Close) {
        window_.setView(view_);
        pushState(std::make_unique<NameState>());
}

void resetZoom(sf::RenderWindow& window, sf::View& view, int& currentZoom) {
    view.reset(sf::FloatRect(0.f, 0.f, 1366.f, 768.f));
    currentZoom = 0;
    window.setView(view);
}

void Game::run() {
    // Game loop
    sf::Clock clock;
    sf::Event event;
    while (window_.isOpen()){
        sf::Time deltaTime = clock.restart();
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
                break;
            }
            // as long as there is at least menu state in stack
            else if (!states_.empty()) {
                std::pair<std::string, int> action = states_.top()->processEvent(event, window_, view_);
                // if level button pressed in main menu
                if (action.first == "open") {
                    // std::cout << "opening level" << std::endl;
                    resetZoom(window_, view_, currentZoom_);
                    if (dynamic_cast<MenuState*>(states_.top().get())) // to check if this action is from menustate or sandboxstate
                    {
                        pushState(std::make_unique<LevelState>(action.second));
                    }
                    else
                    {
                        changeState(std::make_unique<LevelState>(action.second));
                    }
                    
                }
                // if menu button pressed in a level or gameover screen
                else if (action.first == "menu") {
                    // std::cout << "going back to main menu" << std::endl;
                    resetZoom(window_, view_, currentZoom_);
                    popState();
                }
                // if restart button pressed 
                else if (action.first == "restart" || action.first == "next") {
                    // std::cout << "opening another level" << std::endl;
                    resetZoom(window_, view_, currentZoom_);
                    changeState(std::make_unique<LevelState>(action.second));
                }
                else if (action.first == "savename") {
                    player_name_ = states_.top()->getPlayerName();
                    // std::cout << "saving player name " + player_name_ << std::endl;
                    changeState(std::make_unique<MenuState>());
                }
                else if (action.first == "sandbox")
                {
                    // std::cout << "opening sandbox" << std::endl;
                    if (dynamic_cast<MenuState*>(states_.top().get())) // check if the action comes from menustate or opened sandbox levelstate
                    {
                        pushState(std::make_unique<SandboxState>());
                    }
                    else
                    {
                        changeState(std::make_unique<SandboxState>());
                    }
                }
            }
        }
        // Update the current game state
        if (!states_.empty()) {
            std::pair<std::string, int> action = states_.top()->update(deltaTime, window_, view_);
            states_.top()->render(window_, view_);
            // Checks if game is won or lost or ongoing
            if (states_.top()->isLevelState())
            {
                if (action.first == "win") {
                    // std::cout << "game over opening high scores" << std::endl;
                    resetZoom(window_, view_, currentZoom_);
                    int score = states_.top()->calculateScore();
                    int stars = states_.top()->getStars();
                    changeState(std::make_unique<GameOverState>(action.second, true, score, player_name_, stars));
                }
                else if (action.first == "lose") {
                    // std::cout << "game over opening lose screen" << std::endl;
                    resetZoom(window_, view_, currentZoom_);
                    int score = states_.top()->calculateScore();
                    changeState(std::make_unique<GameOverState>(action.second, false, score, player_name_, 0));
                }
            }
        }
    }
}
