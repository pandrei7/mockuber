// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_

#include <fstream>
#include <string>
#include <vector>

#include "./uber_app.h"

class solver {
 public:
    void task1_solver(std::ifstream& fin, std::ofstream& fout);

    void task2_solver(std::ifstream& fin, std::ofstream& fout);

    void task3_solver(std::ifstream& fin, std::ofstream& fout);

    void task4_solver(std::ifstream& fin, std::ofstream& fout);

    void task5_solver(std::ifstream& fin, std::ofstream& fout);

 private:
    UberApp app_;
    std::vector<std::string> inters_;
};
#endif  // SOLVER_H_
