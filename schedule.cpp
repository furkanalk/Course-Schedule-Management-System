#include "schedule.h"

bool isTeacherAvailable(SchoolScheduler& scheduler, std::string teacherName, std::string day, int timeSlot) {
    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    TeacherManagement* teacher = dbHandler->getTeacherByName(teacherName);

    int dayIdx = scheduler.dayIndex[day];

    auto workdays = teacher->getWorkdays();
    return workdays[0][dayIdx] > 0;
}

bool isRoomAvailable(SchoolScheduler& scheduler, std::string day) {
    if (scheduler.dayIndex.find(day) == scheduler.dayIndex.end())
        return true;

    int dayIdx = scheduler.dayIndex[day];

    if (dayIdx < 0 || dayIdx >= 6)
        return false;
}

void Chromosome::calculateFitness(SchoolScheduler& scheduler) {
    std::map<std::string, std::set<int>> usedTimeSlots;
    fitness = 0;

    for (auto gene : genes) {
        bool isTimeSlotFree = usedTimeSlots[gene.day].find(gene.timeSlot) == usedTimeSlots[gene.day].end();

        if (isTimeSlotFree &&
            isTeacherAvailable(scheduler, gene.teacherName, gene.day, gene.timeSlot) &&
            isRoomAvailable(scheduler, gene.day))
        {
            fitness += 1.0;
            usedTimeSlots[gene.day].insert(gene.timeSlot);
        }
    }
    fitness /= genes.size();
}

void CourseSchedulerGA::initializePopulation() {
    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    CourseManagement* allCourses = dbHandler->getCourses();

    std::vector<std::string> days = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };
    std::vector<int> timeSlots = { 9, 11, 13, 15 };

    std::random_device rd;
    std::default_random_engine gen(rd());

    for (int i = 0; i < populationSize; i++) {
        Chromosome newChromosome;
        std::map<int, int> courseScheduleCount;

        for (std::string day : days) {
            for (int timeSlot : timeSlots) {
                std::uniform_int_distribution<int> courseDist(0, allCourses->getIds().size() - 1);
                int courseIndex = courseDist(gen);
                int courseId = allCourses->getIds()[courseIndex];

                if (courseScheduleCount[courseId] >= 4) {
                    continue;
                }

                std::string courseName = allCourses->getNames()[courseIndex];
                std::string teacherName = dbHandler->getTeacherByCourseName(courseName);

                std::vector<std::string> roomNames = allCourses->getRooms()[courseIndex];
                std::uniform_int_distribution<int> roomDist(0, roomNames.size() - 1);
                std::string roomName = roomNames[roomDist(gen)];
                int roomId = dbHandler->getRoomIdByName(roomName);

                Gene gene;
                gene.courseName = courseName;
                gene.teacherName = teacherName;
                gene.roomId = roomId;
                gene.roomName = roomName;
                gene.timeSlot = timeSlot;
                gene.day = day;

                if (isTeacherAvailable(*scheduler, gene.teacherName, day, gene.timeSlot) &&
                    isRoomAvailable(*scheduler, day)) {
                    newChromosome.genes.push_back(gene);
                    courseScheduleCount[courseId]++;
                }
            }
        }

        newChromosome.calculateFitness(*scheduler);
        population.push_back(newChromosome);
    }
}

Chromosome CourseSchedulerGA::selectParent() {
    int tournamentSize = 5;
    std::uniform_int_distribution<int> dist(0, population.size() - 1);

    int bestIndex = dist(generator);
    Chromosome best = population[bestIndex];

    for (int i = 0; i < tournamentSize - 1; i++) {
        int newIndex = dist(generator);
        Chromosome competitor = population[newIndex];
        if (competitor.fitness > best.fitness) {
            best = competitor;
            bestIndex = newIndex;
        }
    }
    return best;
}

Chromosome CourseSchedulerGA::crossover(Chromosome& parent1, Chromosome& parent2) {
    Chromosome offspring;
    std::uniform_int_distribution<int> dist(0, parent1.genes.size() - 1);
    int crossoverPoint = dist(generator);

    for (int i = 0; i < crossoverPoint; ++i)
        offspring.genes.push_back(parent1.genes[i]);
    for (int i = crossoverPoint; i < parent2.genes.size(); ++i)
        offspring.genes.push_back(parent2.genes[i]);

    offspring.calculateFitness(*scheduler);
    return offspring;
}

void CourseSchedulerGA::mutate(Chromosome& chromosome) {
    std::uniform_real_distribution<double> mutationChance(0.0, 1.0);
    std::uniform_int_distribution<int> geneDist(0, chromosome.genes.size() - 1);
    std::vector<int> newTimeSlots = { 9, 11, 13, 15 };
    std::uniform_int_distribution<int> timeSlotDist(0, newTimeSlots.size() - 1);

    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    int roomCount = dbHandler->getRoomCount();
    std::uniform_int_distribution<int> roomDist(0, roomCount - 1);

    for (int i = 0; i < chromosome.genes.size(); ++i) {
        if (mutationChance(generator) <= mutationRate) {
            int geneIndex = geneDist(generator);
            int newTimeSlotIndex = timeSlotDist(generator);
            int newTimeSlot = newTimeSlots[newTimeSlotIndex];
            int newRoomId = roomDist(generator);

            for (auto& otherGene : chromosome.genes) {
                if (otherGene.day == chromosome.genes[geneIndex].day && otherGene.timeSlot == newTimeSlot)
                    return;
            }

            chromosome.genes[geneIndex].timeSlot = newTimeSlot;
            chromosome.genes[geneIndex].roomId = newRoomId;
        }
    }

    chromosome.calculateFitness(*scheduler);
}

void CourseSchedulerGA::createNewGeneration() {
    std::vector<Chromosome> newPopulation;
    std::uniform_real_distribution<double> chance(0.0, 1.0);

    for (int i = 0; i < populationSize; ++i) {
        Chromosome parent1 = selectParent();
        Chromosome parent2 = selectParent();

        Chromosome offspring;
        if (chance(generator) < crossoverRate) {
            offspring = crossover(parent1, parent2);
        }
        else {
            offspring = (chance(generator) < 0.5) ? parent1 : parent2;
        }

        mutate(offspring);
        newPopulation.push_back(offspring);
    }
    population = std::move(newPopulation);
}

Chromosome CourseSchedulerGA::run() {
    initializePopulation();
    int numberOfGenerations = 20;

    double bestFitness = -1.0;
    Chromosome bestChromosome;

    for (int generation = 0; generation < numberOfGenerations; generation++) {
        createNewGeneration();

        for (auto chromosome : population) {
            if (chromosome.fitness > bestFitness) {
                bestFitness = chromosome.fitness;
                bestChromosome = chromosome;
            }
        }
    }

    return bestChromosome;
}