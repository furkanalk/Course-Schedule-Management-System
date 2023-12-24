#include "schedule.h"


void SchoolScheduler::addClassToTeacherSchedule(const std::string& teacherName, const std::string& day, int timeSlot) {
    std::map<std::string, int> dayIndex = { {"monday", 0}, {"tuesday", 1}, {"wednesday", 2},
                                           {"thursday", 3}, {"friday", 4}, {"saturday", 5} };
    TeacherSchedule newClass = { teacherName, day, timeSlot };
    teacherSchedules[dayIndex[day]].push_back(newClass);
}

bool SchoolScheduler::isTeacherAvailable(const std::string& teacherName, const std::string& day, int timeSlot) {
    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    TeacherManagement* teacher = dbHandler->getTeacherByName(teacherName);

    if (!teacher) {
        return false; // Teacher not found
    }

    std::map<std::string, int> dayToIndex = { {"monday", 0}, {"tuesday", 1}, {"wednesday", 2},
                                             {"thursday", 3}, {"friday", 4}, {"saturday", 5} };
    int dayIdx = dayToIndex[day];

    if (dayIdx < 0 || dayIdx >= teacherSchedules.size()) {
        return true; // Day index out of bounds, assuming teacher is available
    }

    if (!teacher->getWorkdays()[dayIdx][timeSlot]) {
        return false; // Teacher not working at this timeslot
    }

    for (const auto& scheduledClass : teacherSchedules[dayIdx]) {
        if (scheduledClass.teacherName == teacherName && scheduledClass.timeSlot == timeSlot) {
            return false; // Teacher already has a class at this timeslot
        }
    }

    return true; // Teacher is available
}


bool SchoolScheduler::isRoomAvailable(int roomId, const std::string& day, int timeSlot) {
    std::map<std::string, int> dayIndex = { {"monday", 0}, {"tuesday", 1}, {"wednesday", 2},
                                           {"thursday", 3}, {"friday", 4}, {"saturday", 5} };

    // Ensure the day is a valid key in the map
    if (dayIndex.find(day) == dayIndex.end()) {
        return true; // Day not found, assume room is available
    }

    int dayIdx = dayIndex[day];

    // Check if dayIdx is within the valid range of the vector
    if (dayIdx >= 0 && dayIdx < weekSchedule.size()) {
        // Iterate over the scheduled classes for the given day
        for (const auto& scheduledClass : weekSchedule[dayIdx]) {
            if (scheduledClass.roomId == roomId && scheduledClass.timeSlot == timeSlot) {
                return false; // Room is not available
            }
        }
    }

    return true; // Room is available
}


void SchoolScheduler::initializeWeekSchedule() {
    for (auto& day : weekSchedule) {
        day.clear();
    }
}

void SchoolScheduler::scheduleClass(int courseId, const std::string& teacherName, int roomId, int timeSlot, const std::string& day) {
    // A map to convert day names to indices
    std::map<std::string, int> dayIndex = { {"monday", 0}, {"tuesday", 1}, {"wednesday", 2},
                                           {"thursday", 3}, {"friday", 4}, {"saturday", 5} };

    // Create and add a new class to the week schedule
    ScheduledClass newClass = { courseId, teacherName, roomId, timeSlot, day };
    weekSchedule[dayIndex[day]].push_back(newClass);

    // Update the teacher's schedule
    addClassToTeacherSchedule(teacherName, day, timeSlot);
}


// Chromosome method implementation
void Chromosome::calculateFitness(SchoolScheduler& scheduler) {
    fitness = 0.0;
    for (auto& gene : genes) {
        if (scheduler.isTeacherAvailable(gene.teacherName, gene.day, gene.timeSlot) &&
            scheduler.isRoomAvailable(gene.roomId, gene.day, gene.timeSlot)) {
            fitness += 1.0;
        }
    }
    fitness /= genes.size();
}

void CourseSchedulerGA::initializePopulation() {
    // Fetch all the necessary data from the database
    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    TeacherManagement* allTeachers = dbHandler->getTeachers();
    CourseManagement* allCourses = dbHandler->getCourses();
    RoomManagement* allRooms = dbHandler->getClassrooms();

    // Define the working days and time slots
    std::vector<std::string> days = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };
    std::vector<int> timeSlots = { 0, 1, 2, 3 }; // Assuming four time slots per day

    // Random number generation setup
    std::random_device rd;
    std::mt19937 gen(rd());

    // Initialize the population with random but valid chromosomes
    for (int i = 0; i < populationSize; ++i) {
        Chromosome newChromosome;

        for (const std::string& day : days) {
            for (int timeSlot : timeSlots) {
                // Randomly select a teacher, course, and room
                std::uniform_int_distribution<> teacherDist(0, allTeachers->getIds().size() - 1);
                std::uniform_int_distribution<> courseDist(0, allCourses->getIds().size() - 1);
                std::uniform_int_distribution<> roomDist(0, allRooms->getIds().size() - 1);

                int teacherIndex = teacherDist(gen);
                int courseIndex = courseDist(gen);
                int roomIndex = roomDist(gen);

                // Create a gene with the selected information
                Gene gene;
                gene.courseId = allCourses->getIds()[courseIndex];
                gene.teacherName = allTeachers->getIds()[teacherIndex];
                gene.roomId = allRooms->getIds()[roomIndex];
                gene.timeSlot = timeSlot;
                gene.day = day;

                // Add the gene to the chromosome if the teacher and room are available
                if (scheduler->isTeacherAvailable(allTeachers->getNames()[teacherIndex], day, timeSlot) &&
                    scheduler->isRoomAvailable(allRooms->getIds()[roomIndex], day, timeSlot)) {
                    newChromosome.genes.push_back(gene);
                }
            }
        }

        // Calculate the fitness for the new chromosome
        newChromosome.calculateFitness(*scheduler);

        // Add the new chromosome to the population
        population.push_back(newChromosome);
    }
}

Chromosome CourseSchedulerGA::selectParent() {
    // Define tournament size, which is how many chromosomes will compete to be a parent
    const size_t tournamentSize = 5; // This can be adjusted
    std::uniform_int_distribution<> dist(0, population.size() - 1);

    // Start with a random chromosome as the current best
    int bestIndex = dist(generator);
    Chromosome best = population[bestIndex];

    // Conduct the tournament
    for (size_t i = 0; i < tournamentSize - 1; ++i) { // We already have one contestant
        int newIndex = dist(generator);
        Chromosome competitor = population[newIndex];
        if (competitor.fitness > best.fitness) {
            best = competitor; // New best found
            bestIndex = newIndex;
        }
    }

    // Return the chromosome that won the tournament
    return best;
}

Chromosome CourseSchedulerGA::crossover(const Chromosome& parent1, const Chromosome& parent2) {
    Chromosome offspring;
    std::uniform_int_distribution<> dist(0, parent1.genes.size() - 1);
    int crossoverPoint = dist(generator);

    for (int i = 0; i < crossoverPoint; ++i) {
        offspring.genes.push_back(parent1.genes[i]);
    }
    for (int i = crossoverPoint; i < parent2.genes.size(); ++i) {
        offspring.genes.push_back(parent2.genes[i]);
    }

    offspring.calculateFitness(*scheduler);
    return offspring;
}

void CourseSchedulerGA::mutate(Chromosome& chromosome) {
    std::uniform_int_distribution<> geneDist(0, chromosome.genes.size() - 1);
    std::uniform_int_distribution<> timeSlotDist(0, 3); // Assuming 4 time slots

    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    int count = dbHandler->getRoomCount(); // Get the total number of rooms
    std::uniform_int_distribution<> roomDist(0, count - 1); // Corrected line

    int geneIndex = geneDist(generator);
    chromosome.genes[geneIndex].timeSlot = timeSlotDist(generator);
    chromosome.genes[geneIndex].roomId = roomDist(generator); // Randomly change room

    chromosome.calculateFitness(*scheduler);
}

void CourseSchedulerGA::createNewGeneration() {
    std::vector<Chromosome> newPopulation;
    for (int i = 0; i < populationSize; ++i) {
        Chromosome parent1 = selectParent();
        Chromosome parent2 = selectParent();
        Chromosome offspring = crossover(parent1, parent2);
        mutate(offspring);
        newPopulation.push_back(offspring);
    }
    population = std::move(newPopulation);
}

Chromosome CourseSchedulerGA::run() {
    for (int generation = 0; generation < 20; ++generation) {
        createNewGeneration();
        // Optionally: Find and log the best chromosome of each generation
    }

    // Find and return the best chromosome in the final generation
    return *std::max_element(population.begin(), population.end(),
        [](const Chromosome& a, const Chromosome& b) {
            return a.fitness < b.fitness;
        });
}
