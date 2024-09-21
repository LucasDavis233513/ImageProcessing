import subprocess
import os

# Convert the images in a given directory to pgm format

Directory = input("Please input the directory of the img(s) you want to convert:\n")

# Convert each .gif file into .pgm file
for file in os.listdir(Directory):
    basename, ext = os.path.splitext(file)
    if ext == '.gif':  # Process only gif files
        with open(f"{Directory}/{basename}.pnm", "wb") as pnm_file:
            subprocess.run(["giftopnm", f"{Directory}/{basename}.gif"], stdout=pnm_file)
        with open(f"{Directory}/{basename}.pgm", "wb") as pgm_file:
            subprocess.run(["ppmtopgm"], stdin=open(f"{Directory}/{basename}.pnm", "rb"), stdout=pgm_file)
        os.remove(f"{Directory}/{basename}.pnm")

ans = input("\nDo you want to remove the old imgs?>(y/n)\n")

# remove the original imgs
if ans == "y":
    for file in os.listdir(Directory):
        basename, ext = os.path.splitext(file)
        if ext == '.gif':
            subprocess.run(["rm", f"{Directory}/{file}"])