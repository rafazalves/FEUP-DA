import random
import subprocess
import os

def create_encomendas(n, pesoMin, pesoMax, volumeMin, volumeMax):
    """
    Escreve para um arquivo de texto, uma encomenda por linha volume peso recompensa duração(s)
    """
    with open("encomendasT.txt", "w") as f:
        f.write("volume peso recompensa duração(s)\n")
        for i in range(n):
            volume = random.randint(volumeMin, volumeMax)
            peso = random.randint(pesoMin, pesoMax)
            recompensa = random.randint(1, 100)
            duracao = random.randint(1, 100)
            f.write("{} {} {} {}\n".format(volume, peso, recompensa, duracao))


def create_carrinhas(n, pesoMin, pesoMax, volumeMin, volumeMax):
    """
    Escreve para um arquivo de texto, uma carrinha por linha volMax pesoMax custo
    """
    with open("carrinhasT.txt", "w") as f:
        f.write("volMax pesoMax custo\n")
        for i in range(n):
            volMax = random.randint(volumeMin, volumeMax)
            pesoMax = random.randint(pesoMin, pesoMax)
            custo = random.randint(1, 100)
            f.write("{} {} {}\n".format(volMax, pesoMax, custo))


def testar_cenario(cenario):
    """
    Executa o arquivo a.out
    """
    for _ in range(30):
        create_encomendas( random.randint(500, 1000), 6, 30, 6, 30)
        create_carrinhas( random.randint(50, 100), 250, 380, 250, 380)
        subprocess.call(["./a.out", "encomendasT.txt", "carrinhasT.txt", cenario])
        os.remove("encomendasT.txt")
        os.remove("carrinhasT.txt")

testar_cenario("1")