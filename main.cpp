#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Car
{
    int id;
    char* name;
};

struct List
{
    Car *car;
    List *previuos;
    List *next;
};


struct Station {
    int id;
    List *queue;
    int *isFree;
    Car *cars;
    int congestion;
    int size;
};


List *createList();
Station *createStation(int id, int size);
void addCarToStationOrQueue(Car *car, Station **station);
void leaveCarFromStation(int id, Station **station);
void showCarsOnStation(Station **station);
void showCarsInQueue(Station **station);

void putCarToQueue(Car *car, Station **station);
Car getCarFromQueue(Station **station);
Car* createRandomCar();
void menu();

int main() {
    srand(time(NULL));
    menu();
    return 0;
}

List *createList()
{
    List *temp = (List*) calloc(1, sizeof(temp[0]));
    temp->next = NULL;
    temp->previuos = NULL;
    temp->car = NULL;
    return temp;
}

Station *createStation() 
{
    int id;
    int size;

    printf("Enter the id of new station\n");
    scanf("%d", &id);

    printf("Enter the size of new station\n");
    scanf("%d", &size);

    Station *station = (Station*) calloc(1, sizeof(station[0]));
    station->id = id;
    station->queue = createList();
    station->isFree = (int*) calloc(size, sizeof(int));
    station->cars = (Car*) calloc(size, sizeof(station->cars[0]));
    station->congestion = size;
    station->size = size;
    return station;
}

void addCarToStationOrQueue(Car *car, Station **station)
{
    printf("There are %d places out of %d left!\n", station[0]->congestion, station[0]->size);
    if(station[0]->congestion > 0)
    {
        printf("These places are free!\n");
        for(int i = 0; i < station[0]->size; i++)
        {
            if(station[0]->isFree[i] == 0)
            {
                printf("The place number %d is free\n", i);
            }
        }
        printf("Enter the the index you want your car to take place. Make sure it is free\n");
        int randomIndex;
        scanf("%d", &randomIndex);
        printf("You decieded to take place number %d\n", randomIndex);
        station[0]->congestion--;
        station[0]->isFree[randomIndex] = 1;
        station[0]->cars[randomIndex] = car[0];
    } else 
    {
        putCarToQueue(car, station);
    }
}

void leaveCarFromStation(Station **station) 
{
    int id;
    printf("Enter the id of car to leave the station\n");
    scanf("%d", &id);

    int isFound = 0;
    int indexToLeave;
    for(int i = 0; i < station[0]->size; i++)
    {
        if(station[0]->isFree[i] == 1)
        {
            if(station[0]->cars[i].id == id)
            {
                indexToLeave = i;
                isFound = 1;
                break;
            }
        }
    }
    if(isFound == 0)
    {
        printf("Such car doesn't exists :(\n");
        return;
    }

    //clear
    Car *temp = &(station[0]->cars[indexToLeave]);
    // free(temp);
    station[0]->cars[indexToLeave].id = NULL;
    station[0]->cars[indexToLeave].name = NULL;
    station[0]->congestion--;
    station[0]->isFree[indexToLeave] = 0;

    //add new car from queue
    if(station[0]->queue->previuos != NULL)
    {
        printf("The queue is not empty\n");
        List *toRemove = station[0]->queue;
        station[0]->congestion++;
        station[0]->isFree[indexToLeave] = 1;
        station[0]->cars[indexToLeave] = getCarFromQueue(station);
        printf("The car with id = %d and with name = %s was put to the station from queue!\n", station[0]->cars[indexToLeave].id,
        station[0]->cars[indexToLeave].name);
        free(toRemove);
    }

}

void showCarsOnStation(Station **station)
{
    printf("There are %d car now on the station\n", station[0]->size - station[0]->congestion);
    for(int i = 0; i < station[0]->size; i++)
    { 
        if(station[0]->isFree[i] == 1) 
        {
            printf("%d place is taken by car with id = %d and name = %s\n", i, station[0]->cars[i].id, station[0]->cars[i].name);
        }
    }
}

void showCarsInQueue(Station **station)
{
    printf("These cars in queue to be get to the station!\n");
    List *iterator = station[0]->queue;
    int position = 1;
    iterator = iterator->previuos;
    while(iterator != NULL)
    {
        printf("Car on position = %d with id = %d and name =  %s\n", position++, iterator->car->id, iterator->car->name);
        iterator = iterator->previuos;
    }
    if(position == 1) 
    {
        printf("The queue is free!\n");
    }
    printf("The end of the queue\n");
}

void putCarToQueue(Car *car, Station **station)
{
    List *newItem = (List*) calloc(1, sizeof(newItem[0]));
    newItem->car = car;

    //find the last car
    List *iterator = station[0]->queue;

    while(true)
    {
        if(iterator->previuos != NULL)
        {
            iterator = iterator->previuos;
        } else 
        {
            break;
        }
    }

    iterator->previuos = newItem;
    // station[0]->queue->previuos = newItem;
    printf("Car with id = %d and name = %s was put to queue!\n", car->id, car->name);
}

Car* createCarWithRandonId()
{
    Car *car = (Car*) calloc(1, sizeof(car[0]));
    car->id = rand() % 10000 + 100000; 
    car->name = (char*) calloc(20, sizeof(car->name[0]));
    printf("Enter a name for your car!\n");    
    scanf("%s", car->name);
    printf("Created a car with id = %d and name = %s\n", car->id, car->name);
    return car;
}

void menu()
{
    printf("First, let's create a Station. You will be asked for id and size\n");
    Station *station = createStation();
    while(true) {
        printf("You can:\n"
            "\t0. Exit\n"
            "\t1. Add a new car to the station\n"
            "\t2. Display all cars in station\n"
            "\t3. Leave a car from station\n"
            "\t4. Display cars in queue\n"
            "Make a suuggestion!\n"
        );
        getchar();
        int suggestion;
        scanf("%d", &suggestion);
        switch (suggestion)
        {
        case 0:
            return;   
        case 1:
            addCarToStationOrQueue(createCarWithRandonId(), &station);
            break;
        case 2:
            showCarsOnStation(&station);
            break;
        case 3:
            leaveCarFromStation(&station);
            break;
        case 4:
            showCarsInQueue(&station);
            break;
        default:
            printf("Bad suggestion! Try one more time\n");
            break;
        }
    }
}

Car getCarFromQueue(Station **station) {
    List *temp = station[0]->queue->previuos;
    Car *toReturn = station[0]->queue->previuos->car;
    station[0]->queue->previuos = station[0]->queue->previuos->previuos;
    return *(toReturn);
}
