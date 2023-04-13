
import speech_recognition as sr
import os

# inicijalizuj recognizer
r = sr.Recognizer()

# definisanje aktivacione reci(u ovom slucaju "telefon") kao i poznatih komandi
hotword = "telefon"
commands = ["otvori kalkulator", "otvori beleške", "otvori internet", "ugasi"]

def perform_action(command):
    # izvrsi komandu
    if "otvori kalkulator" in command:
        os.system("calc.exe")
        print("Otvaram kalkulator...")
    elif "otvori beleške" in command:
        os.system("notepad.exe")
        print("Otvaram beleške...")
    elif "otvori internet" in command:
        os.system("start chrome")
        print("Otvaram internet...")
    elif "ugasi" in command:
        print("Gasim program...")
        return False
    else:
        print("Nepoznata komanda.")
    return True

def voice_command_recognition():
    print("Slušam...")
    with sr.Microphone() as source:
        # simi govor sa mikrofona u odredjenom vremenu (u ovom slucaju 5 sekundi)
        audio_data = r.record(source, duration=5)

        try:
            # konvertuj glas u tekst
            text = r.recognize_google(audio_data, language="sr-SB")
        except sr.UnknownValueError:
            print("Ništa nije rečeno.")
            return True

        print(f"Rekli ste: {text}")

        # proveri da li je aktivaciona rec sadrzana u prepoznatom tekstu
        if hotword in text.lower():
            # izbaci aktivacionu rec iz teksta
            text = text.lower().replace(hotword, "").strip()

            # prepoznaj da li prepoznat tekst sadrzi neku od komandi
            if any(command in text.lower() for command in commands):
                # izvrsi komandu
                return perform_action(text.lower())
            else:
                print("Nepoznata komanda.")
        else:
            print("Aktivaciona reč nije prepoznata.")
        
        return True

# initial call to the voice command recognition function
while voice_command_recognition():
    pass  # continue looping until perform_action() returns False

import cProfile


cProfile.run('voice_command_recognition()', 'output.dat')

import pstats
from pstats import SortKey

with open("output_time.txt","w") as f:
    p = pstats.Stats("output.dat", stream=f)
    p.sort_stats("time").print_stats()

with open("output_calls.txt", "w")as f:
   p = pstats.Stats("output.dat", stream=f)
p.sort_stats("calls").print_stats()

