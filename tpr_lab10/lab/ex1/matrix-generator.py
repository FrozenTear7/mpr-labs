import random

if __name__ == "__main__":
    size = 1024

    A = open("A_cuBLAS.txt", "w")
    # B = open("B_cuBLAS.txt", "w")

    for _ in range(480):
        for _ in range(320):
            A.write(str(random.randint(0, 9)) + "\n")
            # B.write(str(random.randint(0, 9)) + "\n")


    A.close()
    # B.close()
