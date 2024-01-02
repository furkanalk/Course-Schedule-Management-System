#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <vector>
#include <random>
#include <map>
#include <iostream>
#include "TeacherManagement.h"
#include "CourseManagement.h"
#include "RoomManagement.h"
#include "SQLiteHandler.h"
#include <set>

struct Gene {
    std::string courseName;
    std::string teacherName;
    int roomId;
    std::string roomName;
    int timeSlot;
    std::string day;
};

struct SchoolScheduler {
    std::map<std::string, int> dayIndex = {
        {"monday", 0}, {"tuesday", 1}, {"wednesday", 2}, {"thursday", 3}, {"friday", 4}, {"saturday", 5}
    };

    friend bool isTeacherAvailable(SchoolScheduler& scheduler, std::string teacherName, std::string day, int timeSlot);
    friend bool isRoomAvailable(SchoolScheduler& scheduler, std::string day);
};

struct Chromosome {
    std::vector<Gene> genes;
    double fitness;
    void calculateFitness(SchoolScheduler& scheduler);
};

class CourseSchedulerGA {
private:
    SchoolScheduler* scheduler;
    std::vector<Chromosome> population;
    int populationSize;
    double mutationRate;
    double crossoverRate;
    std::default_random_engine generator;

public:
    CourseSchedulerGA(int size, double mutation, double crossover, SchoolScheduler* schedulerObj)
        : populationSize(size), mutationRate(mutation), crossoverRate(crossover), scheduler(schedulerObj) {
        initializePopulation();
    }

    void initializePopulation();
    Chromosome selectParent();
    Chromosome crossover(Chromosome& parent1, Chromosome& parent2);
    void mutate(Chromosome& chromosome);
    void createNewGeneration();
    Chromosome run();
};

#endif