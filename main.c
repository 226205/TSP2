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
void AnnMenu();
int SimAnn(double, double, float);
void TabuMenu();
int TabuSearch(int, int, int);



int  main()
{
    srand(time(NULL));
    chosingfile();
    menu();

//    getche();
}


void chosingfile()
{
    std::string filename = "plik15.txt";
    int choice = 0;
    while(choice != '1' && choice != '2'){
        std::cout << "Wybierz plik do wczytania: \n 1.Wpisz nazwe pliku do wczytania \n 2.Wczytaj domyslny plik tekstowy \"plik15.txt\" \nTwoj wybor: ";
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
            AnnMenu();
            break;
        }
        case '7':
        {
            TabuMenu();
            break;
        }
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


void AnnMenu()
{
    int resultsSpread = 1000;
    int newResult = 0;

    int numberOfRepeats = 0;
    int* results = new int[resultsSpread];
    for(int i = 0; i < resultsSpread; i++)
        results[i] = 0;

    double temperatureMax = 1000;
    double temperatureMin = 1;
    float annealing = 0;
    int choice = -1;
    while (choice != '0'){
        std::cout << "\n Symulowane wyrzazanie, ustaw porzadane parametry:\n 1. temperatureMax = " << temperatureMax << ", temperatureMin = " << temperatureMin << "\n 2. Stopien wyrzazania = " << annealing << "\n 6. Uruchom algorytm\n 8. Wyswietl wyniki\n 9. Wyczysc wyniki\n 0. Powrot do menu glownego\n Twoj wybor: ";
        choice = getche();
        switch(choice){
            case '1':{
                do {
                    std::cout << "\n\nPodaj minimalna temperature: ";
                    std::cin >> temperatureMin;
                    std::cout << "Podaj maksymalna temperature: ";
                    std::cin >> temperatureMax;
                    if(temperatureMax <= temperatureMin) std::cout << "Temperatura minimalna musi byc mniejsza od maksymalnej";
                } while (temperatureMax <= temperatureMin);
                break;}

            case '2':{
                do{
                    std::cout << "\n\nPodaj stopien wyzarzania (mnoznik temperatury przy kazdej kolejnej iteracji podany w formacie 0.xxx)\n 1. Wpisz wlasny\n 2. Wybierz domyslny 0.999\nTwoj wybor:  ";
                    int choice2 = getche();
                    if(choice2 == '1'){
                        std::cout << "\nPodaj stopien wyzarzania (w formacie 0.xxx): ";
                        std::cin >> annealing;
                    }
                    else if (choice2 == '2') annealing = 0.999;
                } while (annealing <= 0 || annealing >= 1);
                break;}

            case '6':{
                if(annealing <= 0 || annealing >= 1)
                    std::cout << "\n Stopien wyrzazania nie zostal ustawiony, sprobuj ponownie.\n";
                else{
                    std::cout << "\n Wpisz ilosc powtorzen: ";
                    std::cin >> numberOfRepeats;
                    clock_t begin = clock();

                    for(int i = 0; i < numberOfRepeats; i++){
                        newResult = SimAnn(temperatureMax, temperatureMin, annealing);
                        results[newResult]++;
                    }
                    std::clock_t end = clock();
                    std::cout<<"\n\n\nCzas wykonania wszystkich prob: " << double(end - begin) / CLOCKS_PER_SEC << "\n\nproba srednio trwala: "<< (double(end - begin) / CLOCKS_PER_SEC) / numberOfRepeats << "\n";
                }
                break;
            }
            case '8':{
                std::cout << "\n Ilosc wystapien poszczegolnych wynikow:\n";
                for(int i = 0; i < resultsSpread; i++)
                    if(results[i] > 0)
                        std::cout << i << ": " << results[i] << "\n";
                break;
            }
            case '9':{
            for(int i = 0; i < resultsSpread; i++)
                results[i] = 0;
            break;
            }
            default:
                break;
        }
    }
    delete [] results;
}


int SimAnn(double temperatureMax, double temperatureMin, float annealing)
{

	int* bestPath = new int[cityamount+1];      //sciezka i waga optymalnego rozwiazania
	int bestCost = INT_MAX;

	int* tempBestPath = new int[cityamount+1];  //sciezka i waga optymalnego rozwiazania tymczasowego
	int tempBestCost = INT_MAX;

    //setTempBestPath
	tempBestPath[0] = tempBestPath[cityamount] = 0;
    for (int i = 1; i < cityamount; i++)
		tempBestPath[i] = i;
	//setBestPath
	for (int i = 0; i < cityamount + 1; i++)
		bestPath[i] = tempBestPath[i];

	int* tempPath = new int [cityamount + 1];

    int cityA, cityB, temp, tempCost, diff;
    int iteracja = 0;
    while (temperatureMax > temperatureMin) {
        iteracja++;
        for (int i = 0; i <= cityamount; i++)
            tempPath[i] = tempBestPath[i];
        do {
            cityA = (rand() % (cityamount - 1)) + 1;
            cityB = (rand() % (cityamount - 1)) + 1;
        } while (cityA == cityB);

        temp = tempPath[cityA];
        tempPath[cityA] = tempPath[cityB];
        tempPath[cityB] = temp;

        tempCost =  diff = 0;
        for(int i = 0; i < cityamount; i++){
            tempCost += distances[tempPath[i]][tempPath[i + 1]];
        }
        diff = tempCost - tempBestCost;

        if(((double)rand() / (RAND_MAX)) < exp((-1)*diff / temperatureMax) || diff < 0){  // zabezpieczenie przed przekreceniem inta
//            std::cout << "\n\n123456\n\n";
            tempBestCost = 0;
            for(int i = 0; i < cityamount; i++){
                tempBestPath[i] = tempPath[i];
                tempBestCost += distances[tempPath[i]][tempPath[i + 1]];
            }
            if(tempCost < bestCost){    //tempCost bo byla liczona dla temp ktory stal sie nowym tempBestPath
                bestCost = tempCost;
                for (int i = 0; i < cityamount + 1; i++)
                    bestPath[i] = tempBestPath[i];
            }
        }
        temperatureMax = temperatureMax * annealing;    // redukcja geometryczna
	}

//    std::cout<<"\n\nNajkrotsza odnaleziona droga przez wszystkie miasta to:\n";
//    for(int i = 0; i < cityamount; i++)
//        std::cout << bestPath[i] << " -> ";
//    std::cout << bestPath[cityamount];
//    std::cout<<"\nJej calkowity dystans wynosi: " << bestCost;
//    std::cout<<"\n\niteracja: "<<iteracja<< "\n\n";

	delete[] tempBestPath;
	delete[] bestPath;
	delete[] tempPath;

	return bestCost;
}


void TabuMenu()
{
    int resultsSpread = 1000;
    int newResult = 0;
    int doDiv = cityamount;

    int numberOfRepeats = 0;
    int* results = new int[resultsSpread];
    for(int i = 0; i < resultsSpread; i++)
        results[i] = 0;

    int numberOfIteration = 100;
    int tabuSize = cityamount;


    int choice = -1;
    while (choice != '0'){
        std::cout << "\n\n Tabu Search, ustaw porzadane parametry:\n 1. Ilosc iteracji programu: " << numberOfIteration << "\n 2. Wielkosc tablicy Tabu: " << tabuSize << "\n 3. Czestotliwosc dywersyfikowania: " << doDiv << "\n 7. Uruchom algorytm\n 8. Wyswietl wyniki\n 9. Wyczysc wyniki\n 0. Powrot do menu glownego\n Twoj wybor: ";
        choice = getche();
        switch(choice){

            case '1':{
                do{
                    std::cout << "\n Podaj liczbe iteracji (musi byc wieksza od zera): ";
                    std::cin >> numberOfIteration;
                }while(numberOfIteration <= 0);
            break;
            }

            case '2':{
                do{
                    std::cout << "\n\nPodaj wielkosc tablicy Tabu\n 1. Wpisz wlasna\n 2. Wybierz domyslna '" << cityamount-1 << "'\nTwoj wybor:  ";
                    int choice2 = getche();
                    if(choice2 == '1'){
                        std::cout << "\nPodaj wielkosc tablicy Tabu ( zakres: 1 -> " << (cityamount-1)*(cityamount-1) /2 - 1 << " : ";
                        std::cin >> tabuSize;
                    }
                    else if (choice2 == '2') tabuSize = cityamount-1;
                    else    std::cout << "\nCos poszlo zle, sprobuj ponownie.";
                } while (tabuSize < 1 || tabuSize > (cityamount-1)*(cityamount-1) /2 -1 );
                break;}

            case '3':{
                do {
                    std::cout << "\n\nPodaj po jakiej liczbie bezowocnych iteracji ma nastapic dywersyfikacja (min = 2 ): ";
                    std::cin >> doDiv;
                } while (doDiv < 2);
                break;}

            case '7':{
                if(tabuSize < 1 || tabuSize > (cityamount-1)*(cityamount-1) /2 -1)
                    std::cout << "\n Wielkosc tablicy Tabu nie zostala ustawiona, sprobuj ponownie.\n";
                else{
                    std::cout << "\n Wpisz ilosc powtorzen: ";
                    std::cin >> numberOfRepeats;
                    clock_t begin = clock();

                    for(int i = 0; i < numberOfRepeats; i++){
                        newResult = TabuSearch(numberOfIteration, tabuSize, doDiv);
                        results[newResult]++;
                    }
                    std::clock_t end = clock();
                    std::cout<<"\n\n\nCzas wykonania wszystkich prob: " << double(end - begin) / CLOCKS_PER_SEC << "\n\nproba srednio trwala: "<< (double(end - begin) / CLOCKS_PER_SEC) / numberOfRepeats << "\n";
                }
                break;
            }
            case '8':{
                std::cout << "\n Ilosc wystapien poszczegolnych wynikow:\n";
                for(int i = 0; i < resultsSpread; i++)
                    if(results[i] > 0)
                        std::cout << i << ": " << results[i] << "\n";
                break;
            }
            case '9':{
            for(int i = 0; i < resultsSpread; i++)
                results[i] = 0;
            break;
            }
            default:
                break;
        }
    }
    delete [] results;
}


int TabuSearch(int numberOfIteration, int tabuSize, int doDiv)
{
    int** tabuList = new int*[cityamount];             // inicjalizacja tablicy Tabu w rozmiarze x*x
    for(int i = 1; i < cityamount; i++)
        tabuList[i] = new int[cityamount];

    for(int i = 1; i < cityamount; i++)                 // pierwotne zerowanie tabu
        for(int j = 1; j < cityamount; j++)
            tabuList[i][j] = 0;


    int temp = 0;

	int* bestPath = new int[cityamount + 1];      //sciezka i waga optymalnego rozwiazania
	int bestCost = INT_MAX;

	int* tempBestPath = new int[cityamount + 1];  //sciezka i waga optymalnego rozwiazania tymczasowego
	int tempBestCost = INT_MAX;
    tempBestPath[0] = tempBestPath[cityamount] = 0;

    int* tempPath = new int [2];                    // sciezka i waga aktualnie badanego sasiedztwa
    int tempCost = INT_MAX;
    int bestResult = INT_MAX;

    int divers= doDiv;                          // w ktorym momencie wykonac dywersyfikacje

    for(int q = 0; q < numberOfIteration; q++)
    {
        if(divers == doDiv)
        {
            //setTempBestPath on random sequence

            bool* random = new bool[cityamount + 1];            //stworzenie tablicy mowiacej czy dana liczba juz wystapila w sekwencji
            random[0] = random[cityamount] = true;              //podpisanie pierwszego i ostatniego elementu jako juz wykonanego
            for(int i = 1; i < cityamount; i++)                 //podpisanie pozostalych jako niewykonanych
                random[i] = false;
            int tempR, fA, tA;

            for (int i = 1; i < cityamount; i++){               //wykonywanie kolejnych miejsc sekwencji
                fA = tA = 0;
                tempR = (rand() % (cityamount - i)) + 1;        //tempR przyjmuje wartosc losowa z zakresu od 1 do ilosci miejsc pozostalych do zapisu
                for(int j = 1; fA < tempR; j++){                //wykonanie petli tak dlugo, az znajdziemy tyle miejsc nieprzypisanych ile chce nasza wartosc tempR
                    if(random[j] == false)                      //jesli wartosc jeszcze nie wystapila to zwiekszamy falseAmount, potrzebna nam jest wartosc liczby z randa
                        fA++;
                    else                                        //jesli wartosc juz wystapila to zwiekszamy trueAmount, musimy wiedziec ile liczb skipowalismy aby dodac potem ta ilosc do wpisywanej liczby
                        tA++;
                }
                random[tempR + tA] = true;                      //wpisanie liczby do sekwencji oraz oznaczenie jej jako wykorzystanej
                tempBestPath[i] = tempR + tA;
            }
            delete[] random;
            tempBestCost = 0;
            divers = 0;

            for(int i = 1; i < cityamount; i++)                 // zerowanie listy tabu
                for(int j = i+1; j < cityamount; j++)
                    tabuList[i][j] = 0;

            for(int i = 0; i < cityamount; i++)                 // obliczanie odleglosci nowej sciezki
                tempBestCost += distances[tempBestPath[i]][tempBestPath[i + 1]];
        }
        else
        {
            tempCost= INT_MAX;
            for(int i = 1; i < cityamount-1; i++)
                for(int j = i+1; j < cityamount; j++){

                    bestResult = 0;
                    // wyliczanie roznicy w koszcie, obliczajac jedynie roznice miedzy sciezkami bedacymi czesciami zamienionych miast
                    if(i+1 == j){
                        bestResult = distances[tempBestPath[i-1]][tempBestPath[j]] + distances[tempBestPath[j]][tempBestPath[i]] + distances[tempBestPath[j]][tempBestPath[i]] + distances[tempBestPath[i]][tempBestPath[j+1]] - distances[tempBestPath[i-1]][tempBestPath[i]] - distances[tempBestPath[i]][tempBestPath[i+1]] - distances[tempBestPath[j-1]][tempBestPath[j]] - distances[tempBestPath[j]][tempBestPath[j+1]];
//                        std::cout << "\n " << i << "  " << j << "  cost: " << bestResult << " = " << distances[tempBestPath[i-1]][tempBestPath[j]] << " + " << distances[tempBestPath[j]][tempBestPath[i]] << " + " << distances[tempBestPath[j]][tempBestPath[i]] << " + " << distances[tempBestPath[i]][tempBestPath[j+1]] << " - " << distances[tempBestPath[i-1]][tempBestPath[i]] << " - " << distances[tempBestPath[i]][tempBestPath[i+1]] << " - " << distances[tempBestPath[j-1]][tempBestPath[j]] << " - " << distances[tempBestPath[j]][tempBestPath[j+1]];
                    }
                    else{
                        bestResult = distances[tempBestPath[i-1]][tempBestPath[j]] + distances[tempBestPath[j]][tempBestPath[i+1]] + distances[tempBestPath[j-1]][tempBestPath[i]] + distances[tempBestPath[i]][tempBestPath[j+1]] - distances[tempBestPath[i-1]][tempBestPath[i]] - distances[tempBestPath[i]][tempBestPath[i+1]] - distances[tempBestPath[j-1]][tempBestPath[j]] - distances[tempBestPath[j]][tempBestPath[j+1]];
//                        std::cout << "\n " << i << "  " << j << "  cost: " << bestResult << " = " << distances[tempBestPath[i-1]][tempBestPath[j]] << " + " << distances[tempBestPath[j]][tempBestPath[i+1]] << " + " << distances[tempBestPath[j-1]][tempBestPath[i]] << " + " << distances[tempBestPath[i]][tempBestPath[j+1]] << " - " << distances[tempBestPath[i-1]][tempBestPath[i]] << " - " << distances[tempBestPath[i]][tempBestPath[i+1]] << " - " << distances[tempBestPath[j-1]][tempBestPath[j]] << " - " << distances[tempBestPath[j]][tempBestPath[j+1]];
                    }

//                    std::cout << "\n j: " << tempBestPath[j] << " i: " << tempBestPath[i];        //wypisywanie numerow zamienionych miast aby poprawnie uzyc tabuList
                    if(tempBestPath[i] > tempBestPath[j]){                                          //tabuList dziala tylko po jednej stronie przekatnej macierzy przejsc, oby oszczedzic na ilosci operacji. wybieramy te strone realizujac podanego ifa
//                        std::cout << "  j < i";
                        if(tabuList[tempBestPath[j]][tempBestPath[i]] == 0 || bestResult < 0){      //sprawdzenie czy wygenerowany sasiad nie nalezy do tabu badz spelnia kryterium aspiracji
                            if(bestResult <= tempCost){                                             //sprawdzenie czy wygenerowany sasiad jest lepszy od juz sprawdzonych
                                tempPath[0] = i;
                                tempPath[1] = j;
                                tempCost = bestResult;
                        }    }
                    }
                    else{                                                                           //to samo co w if z ta roznica, ze zostala wybrana komorka tablicy znajduja sie symetrycznie do przekatnej macierzy, aby uzywac tylko polowy tablicy
//                        std::cout << "  j > i";
                        if(tabuList[tempBestPath[i]][tempBestPath[j]] == 0 || bestResult < 0)
                            if(bestResult <= tempCost){
                                tempPath[0] = i;
                                tempPath[1] = j;
                                tempCost = bestResult;}
                    }
                }

            temp = tempBestPath[tempPath[0]];                               //zamiana najlepszych miast miejscami
            tempBestPath[tempPath[0]] = tempBestPath[tempPath[1]];
            tempBestPath[tempPath[1]] = temp;

//            std::cout << "\n\n";                                        //sprawdzanie nowej, najlepszej sciezki
//            for(int i = 0; i < cityamount; i++)
//                std::cout << tempBestPath[i] << " -> ";
//            std::cout << tempBestPath[cityamount] << "  tempBestCost: " << tempBestCost << "\n zamiana miejsca: " << tempPath[0] << " z " << tempPath[1];

            temp = 0;
            for(int i = 0; i <cityamount; i++)
                temp += distances[tempBestPath[i]][tempBestPath[i + 1]];
//            std::cout << "   Wyliczony nowy koszt: " << temp;         //wypis kosztu nowej sciezki

            tempBestCost = temp;                        //dekrementacja tabu oraz dopisanie nowego przejscia do listy

            for(int i = 1; i < cityamount; i++){         //dekrementacja tabuList
                for(int j = i + 1; j < cityamount; j++)
                    if(tabuList[i][j] != 0)
                        tabuList[i][j] -= 1;
            }

            if(tempBestPath[tempPath[0]] < tempBestPath[tempPath[1]])                           // dodanie elementu do tabuList o wartosci tabuSize
                tabuList[tempBestPath[tempPath[0]]][tempBestPath[tempPath[1]]] = tabuSize;
            else
                tabuList[tempBestPath[tempPath[1]]][tempBestPath[tempPath[0]]] = tabuSize;

        }
//        std::cout << "\n\n";
//        for(int i = 1; i < cityamount; i++){               // wypisanie tabuList
//            for(int j = 1; j < cityamount; j++){
//                std::cout << tabuList[i][j] << " ";}
//           std::cout << "\n";}

        //ustawienie nowego BestPath

        if( bestCost > tempBestCost)                    //zapisanie nowego najlepszego wyniku, badz zaznaczenie kolejnego z rzedu przejscia petli bez niego, aby wywolac funkcje dywersyfikacji
        {
            bestCost = tempBestCost;
            divers = 0;
            for (int i = 0; i < cityamount + 1; i++)
                bestPath[i] = tempBestPath[i];
        }
        else
            divers++;

    }

//    std::cout<<"\n\nNajkrotsza odnaleziona droga przez wszystkie miasta to:\n";           // wypisanie wyników procesu
//    for(int i = 0; i < cityamount; i++)
//        std::cout << bestPath[i] << " -> ";
//    std::cout << bestPath[cityamount];
//    std::cout<<"\nJej calkowity dystans wynosi: " << bestCost;

    delete[] tempBestPath;
	delete[] bestPath;
	delete[] tempPath;

return bestCost;
}
