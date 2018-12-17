#include <fstream>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <queue>
#include <vector>
#include <ctime>


int cityamount=0;
int **distances;
bool divers = true;


void chosingfile();
bool fileread(std::string);
void Writetab(int**);
void menu();
//class Annealing();
void SimAnn(double, double, float);



int  main()
{
    srand(time(NULL));
    chosingfile();
    menu();

//    getche();
}


void chosingfile()
{
    std::string filename = "plik.txt";
    int choice = 0;
    while(choice != '1' && choice != '2'){
        std::cout << "Wybierz plik do wczytania: \n 1.Wpisz nazwe pliku do wczytania \n 2.Wczytaj domyslny plik tekstowy \"plik.txt\" \nTwoj wybor: ";
        choice = getche(); // pobieranie jednego znaku z klawiatury w formie kodu ascii
        switch (choice){
            case '1':
                std::cout << "\nPodaj nazwe pliku: ";
                std::cin >> filename;
                if(fileread(filename) == 0){
                    std::cout << "Nie udalo sie wczytac pliku o podanej nazwie, sprobuj ponownie! \n";
                    choice = 0;
                }
                break;
            case '2':
                if(fileread(filename) == 0){
                    std::cout << "\nNie udalo sie wczytac pliku, sproboj ponownie! \n";
                    choice = 0;
                }
                break;
            default:
                std::cout << "\nWybrano zla liczbe, prosze powtorzyc! \n";
                break;
        }
    }
    std::cout << "\nPlik " << filename << " zostal wczytany poprawnie. \nZawarte w nim dane to: \n";
    Writetab(distances);
}

bool fileread(std::string filename)         // funkcja do odczytywania danych z pliku
{
    std::fstream plik;
    plik.open(filename,std::ios::in);
    if(plik.good())                         // sprawdzenie poprawnosci wczytania pliku, w razie niepowodzenia zwracamy blad
    {
        plik>>cityamount;
        distances = new int* [cityamount];   // dwuwymiarowa tablica dynamiczna
        for(int i=0;i<cityamount;++i)
            distances[i]=new int[cityamount];
        for(int i=0;i<cityamount;++i)       // zapelnianie tablicy danymi z pliku
            for(int j=0;j<cityamount;++j)
                plik>>distances[i][j];
        return 1;
    }
    return 0;
}

void Writetab(int** cities)
{
    std::cout << "\n\n";
    std::cout << cityamount << '\n';
    for(int i=0;i<cityamount;++i)
    {
        for(int j=0;j<cityamount;++j)
            std::cout<<cities[i][j]<<" ";
        std::cout<<"\n";
    }
}

//struct Annealing {
//        int *bestPath;        //sciezka i waga optymalnego rozwiazania
//        int bestCost;
//        int *tempBestPath;        //sciezka i waga optymalnego rozwiazania tymczasowego
//        int tempBestCost;
//        double temperatureMax;          //temperatura max
//        double temperatureMin;          //temperatira min
//
//        Annealing() {};
//
//        double decreaseTemperature();
//        int simulatedAnnealingAlgorithm(double temperatureMax, double temperatureMin);
//
//        int* setBestPath();
//        int* setTempBestPath();
//
//        int* RandomSwap(int* path);
//        double Probability(int temperature, int* pathA, int* pathB);
//
////        ~Annealing();
//};

void menu()
{
    int choice = -1;
    while(choice != '0')
    {
//        std::cout << "\n\n Aktualne parametry: \nDywersyfikacja: " << divers;
        std::cout << "\n Wybierz opcje do rozwiazania TSP: \n1. Wybierz plik \n6. Symulowane wyzazanie (Annealing) \n7. Lista z zakazami (Tabu Search) \n0. Wyjscie \nTwoj wybor: ";

        choice = getche();
        switch(choice)
        {
        case '1':
        {
            for(int i=0;i<cityamount;++i)        // zwolnij pamiec
                delete[] distances[i];
            delete[] distances;
            std::cout <<"\n\n";
            chosingfile();
            break;
        }
        case '6':
        {
//            Annealing* annealing = new Annealing();
            double temperatureMax;
            double temperatureMin;
            float annealing = 0;
            do {
                std::cout << "\n\nPodaj minimalna temperature: ";
                std::cin >> temperatureMin;
                std::cout << "Podaj maksymalna temperature: ";
                std::cin >> temperatureMax;
                if(temperatureMax <= temperatureMin) std::cout << "Temperatura minimalna musi byc mniejsza od maksymalnej";
            } while (temperatureMax <= temperatureMin);


            do{
                std::cout << "\n\nPodaj stopien wyzarzania (mnoznik temperatury przy kazdej kolejnej iteracji podany w formacie 0.xxx)\n 1. Wpisz wlasny\n 2. Wybierz domyslny 0.999\nTwoj wybor:  ";
                int choice2 = getche();
                if(choice2 == '1'){
                    std::cout << "\nPodaj stopien wyzarzania (w formacie 0.xxx): ";
                    std::cin >> annealing;
                }
                else if (choice2 == '2') annealing = 0.999;
                else    std::cout << "\nCos poszlo zle, sprobuj ponownie.";
            } while (annealing < 0 && annealing > 1);

            SimAnn(temperatureMax, temperatureMin, annealing);
            break;
        }
        case '7':

            break;
        case '0':
            break;
        default:
            {
            std::cout << " \nZly wybor, sprobuj ponownie";
            break;
            }
        }
    }
}

void SimAnn(double temperatureMax, double temperatureMin, float annealing)
{

	int* bestPath = new int[cityamount+1];      //sciezka i waga optymalnego rozwiazania
	int bestCost = INT_MAX;

	int* tempBestPath = new int[cityamount+1];  //sciezka i waga optymalnego rozwiazania tymczasowego
	int tempBestCost;
    clock_t begin = clock();

    //setTempBestPath
	tempBestPath[0] = tempBestPath[cityamount] = 0;
    for (int i = 1; i < cityamount; i++)
		tempBestPath[i] = i;
	//setBestPath
	for (int i = 0; i < cityamount + 1; i++)
		bestPath[i] = tempBestPath[i];

//	for (int i = 0; i < cityamount + 1; i++) std::cout << " " << bestPath[i];

	int* tempP = new int [cityamount + 1]; // dobry rozmiar????????????????????
    int cityA, cityB, temp, sumA, sumB, diff;
    int iteracja = 0;
    while (temperatureMax > temperatureMin) {
        iteracja++;
        for (int i = 0; i <= cityamount; i++)
            tempP[i] = tempBestPath[i];
        do {
            cityA = (rand() % (cityamount - 1)) + 1;
            cityB = (rand() % (cityamount - 1)) + 1;
        } while (cityA == cityB);

//        std::cout << "\n A: "<<cityA<<" B "<<cityB;

        temp = tempP[cityA];
        tempP[cityA] = tempP[cityB];
        tempP[cityB] = temp;



        sumA = sumB = diff = 0;
        for(int i = 0; i < cityamount; i++){
            sumA += distances[tempP[i]][tempP[i + 1]];
            sumB += distances[tempBestPath[i]][tempBestPath[i + 1]];
        }
        diff = sumA - sumB;


//        std::cout <<"  diff: " << diff << "  exp: " << exp((-1)*diff / temperatureMax) << " Tmax:  " << temperatureMax;

        if(((double)rand() / (RAND_MAX)) < exp((-1)*diff / temperatureMax) || diff < 0){  // zabezpieczenie przed przekreceniem inta
//            std::cout << "\n\n123456\n\n";
            for(int i = 0; i < cityamount; i++)
                tempBestPath[i] = tempP[i];
            if(sumA < bestCost){    //sumA bo byla liczona dla temp ktory stal sie nowym tempBestPath
                bestCost = sumA;
                for (int i = 0; i < cityamount + 1; i++)
                    bestPath[i] = tempBestPath[i];
            }
        }
        temperatureMax = temperatureMax * annealing;
	}

    std::clock_t end = clock();
    std::cout<<"\n\nNajkrotsza odnaleziona droga przez wszystkie miasta to:\n";
    for(int i = 0; i < cityamount; i++)
        std::cout << bestPath[i] << " -> ";
    std::cout << bestPath[cityamount];
    std::cout<<"\nJej calkowity dystans wynosi: " << bestCost;
    std::cout<<"\nCzas: " << double(end - begin) / CLOCKS_PER_SEC << "\n\niteracja: "<<iteracja<< "\n\n";

	delete[] tempBestPath;
	delete[] bestPath;
}
