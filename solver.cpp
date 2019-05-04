// Copyright 2019 Preda Andrei

#include "./solver.h"

#include <cstddef>
#include <iomanip>
#include <string>
#include <vector>

#include "./uber_driver.h"

static std::vector<std::string> Tokenize(const std::string &str)
{
    std::vector<std::string> tokens;
    std::size_t pos_last = 0;
    std::size_t pos_curr = str.find(' ');

    while (pos_curr != std::string::npos) {
        tokens.push_back(str.substr(pos_last, pos_curr - pos_last));
        pos_last = pos_curr + 1;
        pos_curr = str.find(' ', pos_last);
    }
    tokens.push_back(str.substr(pos_last));

    return tokens;
}

void solver::task1_solver(std::ifstream &fin, std::ofstream &fout)
{
    int intersections, streets;
    fin >> intersections >> streets;
    fin.get();

    for (auto i = 0; i < intersections; i += 1) {
        std::string name;
        fin >> name;
        app_.AddIntersection(name);
    }
    for (auto i = 0; i < streets; i += 1) {
        std::string a, b;
        fin >> a >> b;
        app_.AddStreet(a, b);
    }

    int queries;
    fin >> queries;
    fin.get();

    for (auto i = 0; i < queries; i += 1) {
        std::string a, b;
        fin >> a >> b;
        fout << (app_.Distance(a, b) == -1 ? "n" : "y") << "\n";
    }
}

void solver::task2_solver(std::ifstream &fin, std::ofstream &fout)
{
    int queries;
    fin >> queries;
    fin.get();

    for (auto i = 0; i < queries; i += 1) {
        std::string a, b;
        fin >> a >> b;
        fout << app_.Distance(a, b) << "\n";
    }
}

void solver::task3_solver(std::ifstream &fin, std::ofstream &fout)
{
    int queries;
    fin >> queries;
    fin.get();

    for (auto i = 0; i < queries; i += 1) {
        std::string line;
        getline(fin, line);

        auto tok = Tokenize(line);
        if (tok[0] == "c") {
            if (tok[3] == "0") {
                app_.AddStreet(tok[1], tok[2]);
            } else if (tok[3] == "1") {
                app_.RemoveStreet(tok[1], tok[2]);
                app_.RemoveStreet(tok[2], tok[1]);
            } else if (tok[3] == "2") {
                app_.AddStreet(tok[1], tok[2]);
                app_.AddStreet(tok[2], tok[1]);
            } else {
                app_.ReverseStreet(tok[1], tok[2]);
            }
        } else {
            if (tok[3] == "0") {
                auto dist = app_.Distance(tok[1], tok[2]);
                fout << (dist == -1 ? "n" : "y") << "\n";
            } else if (tok[3] == "1") {
                fout << app_.Distance(tok[1], tok[2]) << "\n";
            } else {
                auto dist1 = app_.Distance(tok[1], tok[4]);
                auto dist2 = app_.Distance(tok[4], tok[2]);

                auto dist = (dist1 == -1 || dist2 == -1 ? -1 : (dist1 + dist2));
                fout << dist << "\n";
            }
        }
    }
}

static void PrintInfo(const UberDriver &driver, std::ofstream &fout)
{
    fout << driver.Name() << ": " << driver.Location() << " ";
    fout << std::fixed << std::setprecision(3) << driver.Rating() << " ";
    fout << driver.Trips() << " " << driver.Distance() << " ";
    fout << (driver.Online() ? "online" : "offline") << "\n";
}

static void PrintDrivers(const std::vector<UberDriver> &vec,
                         const std::string &type, std::ofstream &fout)
{
    for (const auto &driver : vec) {
        fout << driver.Name() << ":";

        fout << std::fixed << std::setprecision(3);
        if (type == "top_rating") {
            fout << driver.Rating() << " ";
        } else if (type == "top_dist") {
            fout << driver.Distance() << " ";
        } else {
            fout << driver.Trips() << " ";
        }
    }
    fout << "\n";
}

void solver::task4_solver(std::ifstream &fin, std::ofstream &fout)
{
    int queries;
    fin >> queries;
    fin.get();

    for (auto i = 0; i < queries; i += 1) {
        std::string type;
        fin >> type;

        if (type == "d") {
            std::string name, location;
            fin >> name >> location;
            app_.GoOnline(name, location);
        } else if (type == "b") {
            std::string name;
            fin >> name;
            app_.GoOffline(name);
        } else if (type == "r") {
            std::string a, b;
            fin >> a >> b;

            double rating;
            fin >> rating;
            fin.get();

            auto message = app_.MakeTrip(a, b, rating);
            if (!message.empty()) {
                fout << message << "\n";
            }
        } else if (type == "info") {
            std::string name;
            fin >> name;
            PrintInfo(app_.Driver(name), fout);
        } else {
            int num;
            fin >> num;
            fin.get();

            auto drivers = app_.SortedDrivers(type, num);
            PrintDrivers(drivers, type, fout);
        }
    }
}

void solver::task5_solver(std::ifstream &fin, std::ofstream &fout)
{
    int max_dist;
    fin >> max_dist;

    std::string driver;
    fin >> driver;

    int n;
    fin >> n;

    std::vector<std::string> dest(n);
    for (auto &name : dest) {
        fin >> name;
    }

    auto source = app_.Driver(driver).Location();
    auto sorted = app_.SortedByDistance(source, dest, max_dist);

    for (const auto &name : sorted) {
        fout << name << " ";
    }
    fout << "\n";
}
