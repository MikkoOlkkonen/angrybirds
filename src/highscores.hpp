#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

using Scores = std::vector<std::pair<int, std::string>>;

/**
 * @class GameIO
 * @brief Class for reading game informatio from textfile and saving game information to textfile.
 * 
 */
class HighScores {
public:
    /**
     * @brief Construct a new GameIO object
     * 
     * @param filepath Path to textfile being read/written to
     */
    HighScores () 
    : filepath_("../src/textfiles/highscores.txt"),
    scores_(std::vector<Scores>(4, std::vector<std::pair<int, std::string>>(5, {-1, ""})))
    { 
        readFile(); }

    /**
     * @brief Read highscores from textfile to Game object
     * 
     * @return Game object
     */
    void readFile() {
        std::ifstream ifs(filepath_);        // file stream for reading the textfile
        // check if opening file successful
        if (!ifs) {
            throw std::runtime_error("Failed opening the file at " + filepath_ + "!");
        }

        std::string line;                   // string for storing a line
        std::string token;                  // string for storing parts of a line
        int level_ind;                      // index of level in highscores_
        int i;                              // stepper for going through level's high scores

        // get a line from file if not eof
        while(std::getline(ifs, line)) {
            // chech if header line
            if (line == "1" || line == "2" || line == "3" || line == "4") {
                level_ind = std::stoi(line) - 1;
                i = 0;
                // read all lines that belong to a level
                while (std::getline(ifs, line) && i < 5) {
                    if (line == "") {
                        break;
                    }
                    std::istringstream iss(line);   // string stream for handling the line
                    // add parts of line to right places in highscores_
                    if (std::getline(iss, token, ' ')) {
                        scores_[level_ind][i].first = std::stoi(token);
                    }
                    if (std::getline(iss, token)) {
                        scores_[level_ind][i].second = token;
                    i++;
                    }
                }
            }
            else if (line != "") {
                throw std::runtime_error("Corrupted game file at " + filepath_ + "!");
            }
        }
        ifs.close();
    }

    /**
     * @brief Insert new score into levels scores.
     * 
     * @param score Score (name + score) to be insterted.
     * @param level Level to which belongs.
     */
    void insertNew(std::pair<int, std::string> score, int level) {
        int level_ind = level - 1;
        scores_[level_ind].push_back(score);
        sortScores(level_ind);
        // if high score list full, remove last one
        if (scores_[level_ind].size() > 5) {
            scores_[level_ind].pop_back();
        }
        saveToFile();
    }

    /**
     * @brief Sort high scores of a level to descending order of scores.
     * 
     * @param level The level to be sorted.
     */
    void sortScores(int level) {
        std::sort(scores_[level].begin(), scores_[level].end(),
        [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
            return a.first > b.first;
            });
    }

    Scores getHighScores(int level) { return scores_[level-1]; }

    /**
     * @brief Save highscores to textfile
     * 
     * @return std::string filepath
     */
    std::string saveToFile () {
        std::ofstream ofs(filepath_);
        // check if opening file successful
        if (!ofs) {
            throw std::runtime_error("Failed opening the file at " + filepath_ + "!");
        }
        int level_no = 1;
        // read information of each level
        for (auto set : scores_) {
            ofs << level_no << std::endl;       // level number
            // read each score
            for (auto score : set) {
                ofs << score.first << " " << score.second << std::endl;
            }
            level_no++;
            ofs << std::endl;
        }
        ofs.close();
        return filepath_;
    }

    void clearSandBoxScores() {
        scores_[3].clear();
        saveToFile();
    }

private:
    std::string filepath_;
    std::vector<Scores> scores_;
};
