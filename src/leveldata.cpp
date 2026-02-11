#include "leveldata.hpp"
#include <memory>

void LevelData::loadFromFile(int number) {
    std::string filepath("../src/textfiles/level" + std::to_string(number) + ".txt");
    if (number == 4) {
        filepath = "../src/textfiles/sandboxlevel.txt";
    }
    std::ifstream ifs(filepath);    // file stream for reading the level file

    // check if file opened successfully
    if (!ifs) {
        throw std::runtime_error("Failed opening the file" + filepath + "!");
    }
    // create ground and read information on file
    else {
        // Create ground for the level
        ground_ = std::make_shared<Ground>();
        // create variables for storing lines, tokens and properties
        std::string line;
        std::string item;
        std::string prop;
        std::vector<std::string> props;

        // read each line from file stream
        while (std::getline(ifs, line)) {    
            // handle birds section, create and add all birds to vector
            if (line == "Birds") {
                int birdcount = 0;
                while (std::getline(ifs, item) && item != "") {
                    props.clear(); // clear props for each bird
                    // string stream for reading pig properties from item
                    std::istringstream iss(item);
                    // read each property and add to vector
                    while (std::getline(iss, prop, ' ')) {
                        props.push_back(prop);
                    }
                    // add new bird to birds_ based on type
                    if (props[0] == "Red") {
                        std::shared_ptr<RedBird> bird = std::make_shared<RedBird>(40 * birdcount + 130, 585);
                        birds_.push_back(bird);
                    }
                    else if (props[0] == "Yellow") {
                        std::shared_ptr<YellowBird> bird = std::make_shared<YellowBird>(40 * birdcount + 130, 590);
                        birds_.push_back(bird);
                    }
                    else { throw std::runtime_error("Corrupted game file at Birds!"); }
                    birdcount++;
                }
            }

            // handle pigs section, create and add all pigs to vector
            else if (line == "Pigs") {
                while (std::getline(ifs, item) && item != "") {
                    props.clear(); // clear props for each pig
                    // string stream for reading pig properties from item
                    std::istringstream iss(item);
                    // read each property and add to vector
                    while (std::getline(iss, prop, ' ')) {
                        props.push_back(prop);
                    }
                    // add new pig to pigs_ based on read properties in props vector
                    if (props[0] == "King") { 
                        std::shared_ptr<KingPig> pig = std::make_shared<KingPig>(std::stod(props[1]), std::stod(props[2]));
                        pigs_.push_back(pig);
                    }
                    else if (props[0] == "Normal") { 
                        std::shared_ptr<NormalPig> pig = std::make_shared<NormalPig>(std::stod(props[1]), std::stod(props[2]));
                        pigs_.push_back(pig);
                    }
                    else { throw std::runtime_error("Corrupted game file at Pigs!"); }
                }
            }

            // handle obstacles section
            else if (line == "Obstacles") {
                while (std::getline(ifs, item) && item != "") {
                    props.clear(); // clear props for each obstacle
                    // string stream for reading obstacle properties from item
                    std::istringstream iss(item);
                    // read each property and add to vector
                    while (std::getline(iss, prop, ' ')) {
                        props.push_back(prop);
                    }
                    // add new obstacle to obstacles_ based on read properties in props vector
                    if (props[0] == "Wood") { 
                        std::shared_ptr<WoodObstacle> obstacle = std::make_shared<WoodObstacle>(std::stod(props[1]), std::stod(props[2]));
                        obstacles_.push_back(obstacle);
                    }
                    else if (props[0] == "Stone") { 
                        std::shared_ptr<StoneObstacle> obstacle = std::make_shared<StoneObstacle>(std::stod(props[1]), std::stod(props[2]));
                        obstacles_.push_back(obstacle);
                    }
                    else if (props[0] == "Glass") { 
                        std::shared_ptr<GlassObstacle> obstacle = std::make_shared<GlassObstacle>(std::stod(props[1]), std::stod(props[2]));
                        obstacles_.push_back(obstacle);
                    }
                    else { throw std::runtime_error("Corrupted game file at Obstacles!"); }
                }
            }

            else if (line == "Star") {
                while (std::getline(ifs, item) && item != "") {
                    props.clear(); // clear props
                    // string stream for reading star place
                    std::istringstream iss(item);
                    // read each property and add to vector
                    while (std::getline(iss, prop, ' ')) {
                        props.push_back(prop);
                    }
                    // add star based on read properties in props vector
                    star_ = std::make_shared<Star>(std::stod(props[0]), std::stod(props[1]));
                }
            }
        }
    }
}
