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
#include <set>

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
    std::string courseName;
    std::string teacherName;
    int roomId;
    std::string roomName;
    int timeSlot;
    std::string day;
};


class SchoolScheduler {
private:
    std::map<std::string, std::set<int>> occupiedTimeSlots;
    std::vector<std::vector<TeacherSchedule>> teacherSchedules;
    std::vector<std::vector<ScheduledClass>> weekSchedule;

    std::map<std::string, int> dayIndex = { 
        {"monday", 0}, {"tuesday", 1}, {"wednesday", 2}, {"thursday", 3}, {"friday", 4}, {"saturday", 5}
    };
public:
    SchoolScheduler() {
        teacherSchedules.resize(6);
    }

    bool isTimeSlotAvailable(std::string day, int timeSlot);
    void reserveTimeSlot(std::string day, int timeSlot);
    void initializeWeekSchedule();
    void addClassToTeacherSchedule(std::string teacherName, std::string day, int timeSlot);
    void scheduleClass(int courseId, std::string teacherName, int roomId, int timeSlot, std::string day);

    friend bool isTeacherAvailable(SchoolScheduler& scheduler, std::string teacherName, std::string day, int timeSlot);
    friend bool isRoomAvailable(SchoolScheduler& scheduler, int roomId, std::string day, int timeSlot);
};

struct Chromosome {
    std::vector<Gene> genes;
    double fitness;

    void calculateFitness(SchoolScheduler& scheduler);
    std::vector<Gene> getScheduleForDay(std::string day);
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