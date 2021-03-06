#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_MEMBERS 1000
#define MAX_NUMBER_OF_TRIBES 10

typedef struct
{
    char Name[40];
    char Tribe[20];
    char Function[20];
    float PowerWhenRains;
    float PowerWhenIsSunny;
} TribeMember;

typedef struct
{
    char Name[40];
    int NumberOfMembers;
    float TotalPowerOfTribe;
} TribeCharacteristics;

FILE * OpenFile(const char * FileName)
{
    FILE * File = fopen(FileName, "r");

    return File;
}

FILE * ValidateFile(const char * FileName)
{
    FILE * File = OpenFile(FileName);

    if (!File)
    {
        printf("File not found.");
        exit(5);
    }

    return File;
}

int GetNumberOfTribeMembers(FILE * File)
{
    int NumberOfTribeMembers = 0;

    char Line[50];

    while (fgets(Line, 50, File))
        NumberOfTribeMembers += 1;

    return NumberOfTribeMembers;
}

void GetPositionInStruct(TribeMember * TribeMembers, int Index, int Position, char * Word)
{
    if (Position == 1)
        strcpy(TribeMembers[Index].Name, Word);

    if (Position == 2)
        strcpy(TribeMembers[Index].Tribe, Word);

    if (Position == 3)
        strcpy(TribeMembers[Index].Function, Word);

    if (Position == 4)
        TribeMembers[Index].PowerWhenRains = (float)strtod(Word, NULL);

    if (Position == 5)
        TribeMembers[Index].PowerWhenIsSunny = (float)strtod(Word, NULL);
}

TribeMember * GetTribeMembers(FILE * File)
{
    TribeMember * TribeMembers = (TribeMember *)malloc(MAX_NUMBER_OF_MEMBERS * sizeof(TribeMember));

    char Line[50];
    int Index = 0;

    while (fgets(Line, 50, File))
    {
        int Position = 1;
        char * Word = strtok(Line, ",");

        while (Word != NULL)
        {
            GetPositionInStruct(TribeMembers, Index, Position, Word);

            Position += 1;
            Word = strtok(NULL, ",");
        }

        Index += 1;
    }

    return TribeMembers;
}

bool FindTribe(TribeCharacteristics * Tribes, int NumberOfTribes, char * SearchedTribe)
{
    for (int i = 0; i < NumberOfTribes; ++i)
        if (strcmp(Tribes[i].Name, SearchedTribe) == 0)
            return true;

    return false;
}

void IncrementTribeMembers(TribeMember * TribeMembers, TribeCharacteristics * Tribes, int * NumberOfTribes, int Position)
{
    for (int i = 0; i < *NumberOfTribes; ++i)
    {
        if (strcmp(TribeMembers[Position].Tribe, Tribes[i].Name) == 0)
            Tribes[i].NumberOfMembers += 1;
    }
}

TribeCharacteristics * GetTribes(TribeMember * TribeMembers, int NumberOfTribeMembers, int * NumberOfTribes)
{
    TribeCharacteristics * Tribes = (TribeCharacteristics *)calloc(MAX_NUMBER_OF_TRIBES, sizeof(TribeCharacteristics));

    for (int i = 0; i < NumberOfTribeMembers; ++i)
    {
        if (!FindTribe(Tribes, *NumberOfTribes, TribeMembers[i].Tribe))
        {
            strcpy(Tribes[*NumberOfTribes].Name, TribeMembers[i].Tribe);
            Tribes[*NumberOfTribes].NumberOfMembers += 1;

            *NumberOfTribes += 1;
        }
        else
            IncrementTribeMembers(TribeMembers, Tribes, NumberOfTribes, i);
    }

    return Tribes;
}

char * GetWeather(FILE * File)
{
    char * Weather = (char *)malloc(50 * sizeof(char));
    fgets(Weather, 50, File);

    return Weather;
}

int CountRainyDays(char * Weather)
{
    int RainyDays = 0;

    for (int i = 0; i < strlen(Weather); ++i)
        if (Weather[i] == 'r')
            RainyDays += 1;

    return RainyDays;
}

int CountSunnyDays(char * Weather)
{
    int SunnyDays = 0;

    for (int i = 0; i < strlen(Weather); ++i)
        if (Weather[i] == 's')
            SunnyDays += 1;

    return SunnyDays;
}

float GetTotalPower(char * Weather, TribeMember Member)
{
    int RainyDays = CountRainyDays(Weather);
    int SunnyDays = CountSunnyDays(Weather);

    float TotalPower = (RainyDays * Member.PowerWhenRains) + (SunnyDays * Member.PowerWhenIsSunny);

    return TotalPower;
}

void GetTribePower(TribeMember * TribeMembers, int NumberOfTribeMembers, TribeCharacteristics * Tribes, int NumberOfTribes, char * Weather)
{
    for (int i = 0; i < NumberOfTribeMembers; ++i)
        for (int j = 0; j < NumberOfTribes; ++j)
            if (strcmp(TribeMembers[i].Tribe, Tribes[j].Name) == 0)
            {
                if (strcmp(TribeMembers[i].Function, "chieftain") == 0)
                    Tribes[j].TotalPowerOfTribe += 10 * GetTotalPower(Weather, TribeMembers[i]);
                else
                    Tribes[j].TotalPowerOfTribe += GetTotalPower(Weather, TribeMembers[i]);
            }
}

char * FindWinningTribe(TribeCharacteristics * Tribes, int NumberOfTribes)
{
    char * WinningTribe = (char *)malloc(40 * sizeof(char));
    strcpy(WinningTribe, Tribes[0].Name);

    float MaxPower = Tribes[0].TotalPowerOfTribe;

    for (int i = 1; i < NumberOfTribes; ++i)
        if (Tribes[i].TotalPowerOfTribe > MaxPower)
        {
            MaxPower = Tribes[i].TotalPowerOfTribe;
            strcpy(WinningTribe, Tribes[i].Name);
        }

    return WinningTribe;
}

void ShowAllMembers(TribeMember * TribeMembers, int NumberOfTribeMembers)
{
    for (int i = 0; i < NumberOfTribeMembers; ++i)
        printf("%s %s %s %0.2f %0.2f \n", TribeMembers[i].Name, TribeMembers[i].Tribe, TribeMembers[i].Function,
               TribeMembers[i].PowerWhenRains, TribeMembers[i].PowerWhenIsSunny);

    printf("\n");
}

void ShowAllTribes(TribeCharacteristics * Tribes, int NumberOfTribes)
{
    for (int i = 0; i < NumberOfTribes; ++i)
        printf("%s %d %0.2f \n", Tribes[i].Name, Tribes[i].NumberOfMembers, Tribes[i].TotalPowerOfTribe);

    printf("\n");
}

char * EstablishesWeather()
{
    FILE * WeatherFile = ValidateFile("Weather.txt");

    char * Weather = GetWeather(WeatherFile);

    return Weather;
}

void PrepareCompetition(TribeMember ** TribeMembers, int * NumberOfTribeMembers, TribeCharacteristics ** Tribes,
                        int * NumberOfTribes)
{
    FILE * TribesFile = ValidateFile("Tribes.txt");

    *NumberOfTribeMembers = GetNumberOfTribeMembers(TribesFile);

    fseek(TribesFile, 0, SEEK_SET);

    *TribeMembers = GetTribeMembers(TribesFile);
    printf("\nShow all tribe members : \n");
    ShowAllMembers(*TribeMembers, *NumberOfTribeMembers);

    *NumberOfTribes = 0;
    *Tribes = GetTribes(*TribeMembers, *NumberOfTribeMembers, NumberOfTribes);
    printf("Show all tribes : \n");
    ShowAllTribes(*Tribes, *NumberOfTribes);

    char * Weather = EstablishesWeather();

    GetTribePower(*TribeMembers, *NumberOfTribeMembers, *Tribes, *NumberOfTribes, Weather);
    ShowAllTribes(*Tribes, *NumberOfTribes);
}

void EstablishesWinner(TribeCharacteristics * Tribe, int NumberOfTribes)
{
    char * WinningTribe = FindWinningTribe(Tribe, NumberOfTribes);
    printf("The winning tribe is %s.", WinningTribe);
}

int main()
{
    int NumberOfTribeMembers;
    TribeMember * TribeMembers;

    int NumberOfTribes;
    TribeCharacteristics * Tribe;

    PrepareCompetition(&TribeMembers, &NumberOfTribeMembers, &Tribe, &NumberOfTribes);

    EstablishesWinner(Tribe, NumberOfTribes);

    return 0;
}
