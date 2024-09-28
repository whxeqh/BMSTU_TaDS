import random
import sys
import os

'''
Передаваемые аргументы: 
1) Выходной файл
2) Кол-во необходимых записей 
'''

season = [
    "Summer",
    "Fall", 
    "Winter", 
    "Spring"
]

countries = [
    {"country": "Russia", "capital": "Moscow", "continent": "Europe/Asia"},
    {"country": "USA", "capital": "Washington", "continent": "NorthAmerica"},
    {"country": "Canada", "capital": "Ottawa", "continent": "NorthAmerica"},
    {"country": "Brazil", "capital": "Brasilia", "continent": "SouthAmerica"},
    {"country": "Argentina", "capital": "BuenosAires", "continent": "SouthAmerica"},
    {"country": "China", "capital": "Beijing", "continent": "Asia"},
    {"country": "Japan", "capital": "Tokyo", "continent": "Asia"},
    {"country": "India", "capital": "NewDelhi", "continent": "Asia"},
    {"country": "Germany", "capital": "Berlin", "continent": "Europe"},
    {"country": "France", "capital": "Paris", "continent": "Europe"},
    {"country": "UnitedKingdom", "capital": "London", "continent": "Europe"},
    {"country": "Italy", "capital": "Rome", "continent": "Europe"},
    {"country": "Australia", "capital": "Canberra", "continent": "Australia"},
    {"country": "SouthKorea", "capital": "Seoul", "continent": "Asia"},
    {"country": "Mexico", "capital": "MexicoCity", "continent": "NorthAmerica"},
    {"country": "SouthAfrica", "capital": "Pretoria", "continent": "Africa"},
    {"country": "Egypt", "capital": "Cairo", "continent": "Africa"},
    {"country": "SaudiArabia", "capital": "Riyadh", "continent": "Asia"},
    {"country": "Turkey", "capital": "Ankara", "continent": "Asia/Europe"},
    {"country": "Spain", "capital": "Madrid", "continent": "Europe"},
    {"country": "Sweden", "capital": "Stockholm", "continent": "Europe"},
    {"country": "Norway", "capital": "Oslo", "continent": "Europe"},
    {"country": "Finland", "capital": "Helsinki", "continent": "Europe"},
    {"country": "Netherlands", "capital": "Amsterdam", "continent": "Europe"},
    {"country": "Switzerland", "capital": "Bern", "continent": "Europe"},
    {"country": "Belgium", "capital": "Brussels", "continent": "Europe"},
    {"country": "Denmark", "capital": "Copenhagen", "continent": "Europe"},
    {"country": "Poland", "capital": "Warsaw", "continent": "Europe"},
    {"country": "Ukraine", "capital": "Kyiv", "continent": "Europe"},
    {"country": "Greece", "capital": "Athens", "continent": "Europe"},
    {"country": "Portugal", "capital": "Lisbon", "continent": "Europe"},
    {"country": "NewZealand", "capital": "Wellington", "continent": "Australia"},
    {"country": "Indonesia", "capital": "Jakarta", "continent": "Asia"},
    {"country": "Malaysia", "capital": "KualaLumpur", "continent": "Asia"},
    {"country": "Thailand", "capital": "Bangkok", "continent": "Asia"},
    {"country": "Vietnam", "capital": "Hanoi", "continent": "Asia"},
    {"country": "Philippines", "capital": "Manila", "continent": "Asia"},
    {"country": "Kenya", "capital": "Nairobi", "continent": "Africa"},
    {"country": "Nigeria", "capital": "Abuja", "continent": "Africa"},
    {"country": "Ethiopia", "capital": "AddisAbaba", "continent": "Africa"},
    {"country": "Argentina", "capital": "BuenosAires", "continent": "SouthAmerica"},
]


def sightseeing():
    objects_count = random.randint(0, 1000)
    type_of_object = random.randint(1, 3)
    return (objects_count, type_of_object)

def beach():
    seas = season[random.randint(0, 3)]
    air_temperature = random.randint(-1000, 1000)
    water_temperature = random.randint(-1000, 1000)
    return (seas, air_temperature, water_temperature)

def sport():
    return random.randint(1,3)

def get_country():
    switch_dict = {1 : sightseeing(), 2 : beach(), 3 : sport()}

    name = countries[random.randint(0, 39)]["country"]
    capital = countries[random.randint(0, 39)]["capital"]
    mainland = countries[random.randint(0, 39)]["continent"]
    visa = random.randint(0,1)
    flying_time = random.randint(0, 1000)
    min_price = random.randint(0, 1000)
    type_of_tourism = random.randint(1, 3)

    tourism = switch_dict[type_of_tourism]
    tourism_str = " ".join(map(str, tourism)) if isinstance(tourism, tuple) else str(tourism)


    #print(f"COUNTRY: {name} {capital} {mainland} {visa} {flying_time} {min_price}\n")
    #print(f"TYPE: {tourism_str}\n\n")

    return f"{name} {capital} {mainland} {visa} {flying_time} {min_price} {type_of_tourism} {tourism_str}\n"

if __name__ == "__main__":
    file_name = sys.argv[1]
    reps = int(sys.argv[2])

    print(sys.argv)
    current_dir = os.path.dirname(os.path.abspath(__file__))
    output_dir = os.path.join(current_dir, "..", "research") if sys.argv[3] == "-r" else os.path.join(current_dir, "..", "out")

    print(output_dir)

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    file_path = os.path.join(output_dir, file_name)

    with open(file_path, "w") as file:
        while reps:
            country = get_country()
            file.write(country)
            reps -= 1
