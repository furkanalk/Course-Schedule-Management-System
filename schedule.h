#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <iostream>
#include "TeacherManagement.h"
#include "CourseManagement.h"
#include "RoomManagement.h"
#include "SQLiteHandler.h"
#include <string>

struct TeacherSchedule {
    std::string teacherName;
    std::string day;
    int timeSlot;
};

struct ScheduledClass {
    int courseId;
    std::string teacherName;
    int roomId;
    int timeSlot;
    std::string day;
};

struct Gene {
    int courseId;
    std::string teacherName;
    int roomId;
    int timeSlot;
    std::string day;
};


class SchoolScheduler {
private:
    std::vector<std::vector<TeacherSchedule>> teacherSchedules;
    std::vector<std::vector<ScheduledClass>> weekSchedule;
public:
    SchoolScheduler() {
        // Initialize teacherSchedules for each day of the week
        teacherSchedules.resize(6); // Assuming 6 days a week
    }
    void initializeWeekSchedule();
    void addClassToTeacherSchedule(const std::string& teacherName, const std::string& day, int timeSlot);
    void scheduleClass(int courseId, const std::string& teacherName, int roomId, int timeSlot, const std::string& day);
    bool isTeacherAvailable(const std::string& teacherName, const std::string& day, int timeSlot);
    bool isRoomAvailable(int roomId, const std::string& day, int timeSlot);
};

struct Chromosome {
    std::vector<Gene> genes;
    double fitness;

    void calculateFitness(SchoolScheduler& scheduler);
};

class CourseSchedulerGA {
private:
    SchoolScheduler* scheduler;  // Pointer to a SchoolScheduler object
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
    Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2);
    void mutate(Chromosome& chromosome);
    void createNewGeneration();
    Chromosome run();
};

#endif